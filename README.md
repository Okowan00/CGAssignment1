## How to Run (Visual Studio)

1. **Download and unzip** the project from GitHub.
2. **Open** `CGassignment1.sln` in Visual Studio.
3. **Set build config** to `Debug` and `x86`.
4. **Press F5** to build and run.

## Required Settings

- **Include Directory**  
  `Project → Properties → C/C++ → General → Additional Include Directories`  
  → `$(ProjectDir)include`

- **Library Directory**  
  `Project → Properties → Linker → General → Additional Library Directories`  
  → `$(ProjectDir)lib`

- **Linker Input**  
  `Project → Properties → Linker → Input → Additional Dependencies`  
  → `freeglut.lib`

✅ No external installation is required.  
✅ All needed libraries are already included in the project folder.
