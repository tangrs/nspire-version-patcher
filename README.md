# TI-Nspire Version Patcher

Pretty much self-explanatory. Changes the displayed version in the handheld status menu option on the CAS.

![Screenshot](http://i50.tinypic.com/34dljrn.jpg)

Fun for about 5 minutes max.

## Support

I'm lazy so I only found the patch address for 3.1.0.392 CAS CX software. If you want to make this work on your model, here's a quick outline on how to find your patch address (advanced-ish developer knowledge assumed).

First, download Ndless SDK. Look through the syscall definition headers for your calculator and find the address of the 'sprintf' function. It is usually the 12th address in the arm/syscalls_calcmodel_calcversion.c file.

Load your OS into nspire_emu and enter the debugger console. Add a breakpoint for that function by entering ```k 0xaddress +x````.

Then, press 5 and 4 to get to the status screen. Every time the break point is hit, inspect the contents of the address in r1 for something that resembles a format string for a version string (something like ```%d.%s.%s.%d```). Verify it actually builds to the version string by inspecting the contents of r2, r3 and the stack.

When you've found it, get the address in lr and subtract 0x4 from it and that's your patch address. Plug that into the source code and things should work.

## Using

I haven't made a user interface or option setting at the moment. To use it you have to change the source code to what you want and compile it yourself.

You can dump it in the /ndless/startup folder so the patch runs at every startup.

## License

GPLv3.

Use it. If you modify it and distribute it, make sure to include source code.