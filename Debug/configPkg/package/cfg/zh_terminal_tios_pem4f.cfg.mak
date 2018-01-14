# invoke SourceDir generated makefile for zh_terminal_tios.pem4f
zh_terminal_tios.pem4f: .libraries,zh_terminal_tios.pem4f
.libraries,zh_terminal_tios.pem4f: package/cfg/zh_terminal_tios_pem4f.xdl
	$(MAKE) -f C:\Users\saber\workspace_v7\zh_terminal_tios/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\saber\workspace_v7\zh_terminal_tios/src/makefile.libs clean

