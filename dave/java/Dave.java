// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


import com.fazecast.jSerialComm.*;
import javax.sound.midi.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.util.*;
import java.lang.reflect.*;


public class Dave implements Receiver
	{
	/// Dave-specific class code 
	
	SerialPort port;
	MidiDevice midi;
	Object lock = new Object[0];

	public void setMIDI(MidiDevice midi)
		{
		try
			{
			this.midi = midi;
			Transmitter transmitter = midi.getTransmitter();
			transmitter.setReceiver(this);
			midi.open();
			}
		catch (MidiUnavailableException ex)
			{
	        JOptionPane.showMessageDialog(frame, "The MIDI Device " + midi + "could not be set.", "MIDI Device Error", JOptionPane.ERROR_MESSAGE);
			}
		}
		
    /** Returns the byte as a hex number */
    public static String toHex(byte val)
        {
        return String.format("%02X", val);
        }

public static final boolean debug = false;
static Thread thread = null;
static boolean pleaseDie = false;

	public void setSerial(final SerialPort port)
		{
		if (this.port != null) this.port.closePort();
		this.port = port;
		this.port.setBaudRate(31250);
		port.setComPortTimeouts(port.TIMEOUT_READ_SEMI_BLOCKING, 100, 0);
		if (thread != null) 
			{
			// kill old thread if any
			pleaseDie = true;
			try { thread.join(); } catch (InterruptedException ex) { }
			thread = null;
			}
		
//if (debug)
	{
		thread = new Thread(new Runnable()
			{
			public void run()
				{
					byte[] buf = new byte[1];
					while(true)
						{
						if (port.readBytes(buf, 1) == 1)
							System.out.write(buf[0]);
						if (pleaseDie) break;
						}
				}
			});
		thread.setDaemon(true);
		thread.start();
	}

		this.port.openPort();
		}

	public void noteOff(int channel, int note)
		{
		synchronized(lock) { port.writeBytes(new byte[] { (byte)(0x80 + channel), (byte)note, 0x40 }, 3); }
		}
		
	public void panic()
		{
		if (port != null)
			{
			for(int channel = 0; channel < 16; channel++)
				{
				for(int note = 0; note < 127; note++)
					{
					noteOff(channel, note);
					}
				}
			}
		}
	
	
	
	/// Receiver-specific code
	
	public void close() { }
	
	public void send(MidiMessage message, long timestamp)
		{
		if (message instanceof ShortMessage)
			{
			ShortMessage sm = (ShortMessage)message;
			byte[] data = sm.getMessage();
			if (debug)
				{
				for(int i = 0; i < data.length; i++)
					{
					System.out.println(toHex(data[i]));
					}
				}
			if (port != null) synchronized(lock) { port.writeBytes(data, data.length); }
			}
		}





	// Loading Available Devices

    static ArrayList<MidiDevice> getMidiDevices()
        {
        try
        	{
			MidiDevice.Info[] midiDevices = MidiSystem.getMidiDeviceInfo();

			// try coremidi4j instead on MacOS
            try
                {
                Class c = Class.forName("uk.co.xfactorylibrarians.coremidi4j.CoreMidiDeviceProvider");
                Method m = c.getMethod("getMidiDeviceInfo", new Class[0]);
                midiDevices = (MidiDevice.Info[])(m.invoke(null));
                }
            catch (Throwable ex)
                {
                }
		
			ArrayList<MidiDevice> allDevices = new ArrayList<>();
			for(int i = 0; i < midiDevices.length; i++)
				{
				MidiDevice d = MidiSystem.getMidiDevice(midiDevices[i]);
				// get rid of java devices
				if (d instanceof javax.sound.midi.Sequencer ||
					d instanceof javax.sound.midi.Synthesizer)
					continue;
				if (d.getMaxTransmitters() != 0)
					{
					allDevices.add(d);
					}
				}
			return allDevices;
			}
		catch (Exception ex)
			{
			ex.printStackTrace();
			return new ArrayList<MidiDevice>();
			}
		}
		
	static SerialPort[] getSerialDevices()
		{
		return SerialPort.getCommPorts();
		}





	// GUI

	static JFrame frame;
	static Dave dave;
	
     static boolean isUnix() 
        {
    	String OS = System.getProperty("os.name").toLowerCase();
        return (OS.indexOf("nix") >= 0 || OS.indexOf("nux") >= 0 || OS.indexOf("aix") > 0 );
        }

    static String getName(MidiDevice device) 
            { 
            String desc = device.getDeviceInfo().getDescription().trim();
            String name = device.getDeviceInfo().getName();
                        
            if (name == null) 
                name = "";
            if (desc == null || desc.equals("")) 
                desc = "MIDI Device";
                
            if (isUnix()) // Linux names don't permit spaces, so we need the description instead, which is of the form A, B, A
                {
                String[] descs = desc.split(",");
                String[] names = name.split(" ");
                String d = (descs.length > 1 ? descs[0] : desc);
                String n = (names.length > 1 ? names[1] : "(" + names + ")");
                name = d + " " + n;
                }
                
            // All CoreMIDI4J names begin with "CoreMIDI4J - "
            if (name.startsWith("CoreMIDI4J - "))
                name = name.substring(13).trim();
            else
                name = name.trim();

            if (name.equals(""))
                return desc.trim(); 
            else 
                return name;
            }
	
	static void buildSerialCombo(final SerialPort[] ports, JPanel putHere, JFrame frame)
		{
		putHere.removeAll();
		JComboBox combo = new JComboBox(ports);
		if (ports.length > 0)
			{
			dave.port = ports[0];
			dave.setSerial(dave.port);
			combo.setSelectedIndex(0);
			}
		else dave.port = null;
			
		combo.addActionListener(new ActionListener()
			{
			public void actionPerformed(ActionEvent e)
				{
				dave.setSerial(ports[combo.getSelectedIndex()]);
				}
			});
		putHere.add(combo, BorderLayout.CENTER);
		putHere.revalidate();
		putHere.repaint();
		frame.pack();
		}
	
	static void buildMIDICombo(final ArrayList<MidiDevice> devices, JPanel putHere, JFrame frame)
		{
		putHere.removeAll();

		String[] names = new String[devices.size()];
		for(int i = 0; i < names.length; i++)
			{
			names[i] = getName(devices.get(i));
			}
			
		JComboBox combo = new JComboBox(names);

		if (devices.size() > 0)
			{
			dave.midi = devices.get(0);
			dave.setMIDI(dave.midi);
			combo.setSelectedIndex(0);
			}
		else dave.midi = null;

		combo.addActionListener(new ActionListener()
			{
			public void actionPerformed(ActionEvent e)
				{
				dave.setMIDI(devices.get(combo.getSelectedIndex()));
				}
			});
		putHere.add(combo, BorderLayout.CENTER);
		putHere.revalidate();
		putHere.repaint();
		frame.pack();
		}
	
	
	public static void main(String[] args) throws Exception
		{
		dave = new Dave();
		 
		frame = new JFrame("Dave");
		frame.getContentPane().setLayout(new BorderLayout());
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); 
		
		JPanel serialPanel = new JPanel();
		serialPanel.setLayout(new BorderLayout());
		serialPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.GRAY), "GRAINS USB Device"));
		buildSerialCombo(getSerialDevices(), serialPanel, frame);
		frame.getContentPane().add(serialPanel, BorderLayout.CENTER);

		JPanel midiPanel = new JPanel();
		midiPanel.setLayout(new BorderLayout());
		midiPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(Color.GRAY), "MIDI Device"));
		buildMIDICombo(getMidiDevices(), midiPanel, frame);
		frame.getContentPane().add(midiPanel, BorderLayout.SOUTH);
		
		JButton button = new JButton("Reload Devices");
		button.addActionListener(new ActionListener()
			{
			public void actionPerformed(ActionEvent e)
				{
				buildSerialCombo(getSerialDevices(), serialPanel, frame);
				buildMIDICombo(getMidiDevices(), midiPanel, frame);
				}
			});

		JButton panic = new JButton("All Notes Off");
		panic.addActionListener(new ActionListener()
			{
			public void actionPerformed(ActionEvent e)
				{
				dave.panic();
				}
			});
		
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BorderLayout());
		buttonPanel.add(button, BorderLayout.WEST);
		buttonPanel.add(panic, BorderLayout.EAST);
		frame.add(buttonPanel, BorderLayout.NORTH);
		frame.pack();
		frame.setVisible(true);
		}
	}
