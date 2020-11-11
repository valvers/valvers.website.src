# LinuxCNC

LinuxCNC is a project for providing CNC functionality on Linux Platforms.

I've an old i3 that I'm using with a couple of cheap PCIe parallel ports.

In the old world, there would be a parallel port for x86 hardware at 0x378. But in the new world, they are installed at different locations. This is because the old 0x378 address is taken up by a possible on-board parallel port which may or may not be fitted.

Anyway, to get the address of the PCIe parallel port you've installed you can check what the dmesg log has to say for itself. Something like the following:

```
bjs@cnc:~$ dmesg | grep par
[    0.225668] hpet0: 8 comparators, 64-bit 14.318180 MHz counter
[    0.894235] Asymmetric key parser 'x509' registered
[    0.981384] ahci 0000:00:1f.2: flags: 64bit ncq pm led clo pio slum part ems
[    1.013956] xhci_hcd 0000:00:14.0: hcc params 0x200077c1 hci version 0x100 quirks 0x0000000000009810
[    7.363473] PCI parallel port detected: 1c00:3050, I/O at 0xe100(0x0), IRQ 18
[    7.363526] parport0: PC-style at 0xe100, irq 18 [PCSPP,TRISTATE]
[    7.391001] ppdev: user-space parallel port driver
[    7.460152] PCI parallel port detected: 1c00:3050, I/O at 0xc100(0x0), IRQ 16
[    7.460212] parport1: PC-style at 0xc100, irq 16 [PCSPP,TRISTATE]
```

As you can see, I've got two parallel ports installed in this old PC.

## Kernel

Use the latency-test program from the Applications->CNC menu to see what the latency is like on your chosen hardware. For me, the i3 was abysmal. It achieved 10,000,000ns jitter on the base thread. This it totally unusable.

Using the LinuxCNC instructions I installed a replacement RTAI (instead of RT-Preempt) Kernel.

**INSTRUCTIONS**

If you're booting secure UEFI you'll have to disable secure mode here because the signature for the kernel is not valid. You will also have to set the kernel as your default kernel so when you reboot you won't have awful performance again.

## Latency Test Again

Running the latency test again with the new Kernel results in a 12,500ns maximum jitter. This is much better and workable for our system.
