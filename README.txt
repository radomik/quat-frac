1. Download freeglut 2.8.1-1 for MSVC from http://www.transmissionzero.co.uk/software/freeglut-devel/

2. Copy freeglut-MSVC-2.8.1-1.mp\freeglut\lib\freeglut.lib into C:\Program Files\Microsoft Visual Studio 10.0\VC\lib\

3. Copy freeglut-MSVC-2.8.1-1.mp\freeglut\lib\x64\freeglut.lib into C:\Program Files\Microsoft Visual Studio 10.0\VC\lib\amd64

4. Copy folder freeglut-MSVC-2.8.1-1.mp\freeglut\include\GL into C:\Program Files\Microsoft Visual Studio 10.0\VC\include\

5. Copy freeglut-MSVC-2.8.1-1.mp\freeglut\bin\freeglut.dll to folder with 32-bit application executable

6. Copy freeglut-MSVC-2.8.1-1.mp\freeglut\bin\x64\freeglut.dll to folder with 64-bit application executable

Project configuration:
	"Project" -> "<project name> Properties" -> "Configuration Properties" -> "Linker" -> "Input"
	
		On the right, under Additional dependencies (on the beginning) add
			opengl32.lib;glu32.lib;freeglut.lib;
		
