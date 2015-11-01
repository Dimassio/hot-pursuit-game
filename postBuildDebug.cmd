mkdir ..\Debug
mkdir ..\Debug\Resources
cp ..\..\Externals\FreeGLUT\bin\freeglut.dll ..\Debug
xcopy ..\..\Resources ..\Debug\Resources /d /e /y
