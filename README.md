# Hopper
My own operating system built from scratch.
Day 1:
      Installed wsl ubuntu, installed GCC, NASM,QEMU 
      Wrote a boot.nsm file which displayed the name of the os "Hopper" translated it using nasm , used QEMU to run the boot.bin.
      Got the output succesfully:- 
      <img width="845" height="350" alt="image" src="https://github.com/user-attachments/assets/e5baa939-c6cb-456e-88c1-99fbfad11535" />
      /
      /
      #kernal
       * I have created a kernal_entry.asm which points towards the kernal's main class {kernal_entry acts as a gatekeepr showing the direction // location of the kernal placed in the memory}
       * I have created kernal.c it defines kernal_main()
           1.Accesses the VGA text buffer directly at physical address 0xB8000
           2. Clearr the  screen
           3. Prints the message "Hopper kernal is running in 32-bit protected Mode!".
      # Liner
        Since im using a cross - complier i need a linker Script (linker.ld) this tells the compiler exactly where in the physical memory to  place my code so the cpu can find it.
