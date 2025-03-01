set PATH=C:\TurboC++


for %%f in (*.c) do set arg=%cd%\%%~f

COPY "%arg%" "%PATH%\Disk\TurboC3\MAIN.C"
cd "%PATH%"
DOSBox.exe