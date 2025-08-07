CODING STYLE GUIDELINES

This document defines the coding style conventions for this repository. The main goals are clarity, maintainability, and self-documenting code. These rules apply to all C source files in the project.

---

NAMING CONVENTIONS

Variables:
- Use snake_case for all variable names.
- Variable names must be explicit and self-explanatory.
- Avoid single-letter names like i, j, x, y, h, w.
- Use descriptive names such as row, col, height, width.

Functions:
- Function names use snake_case.
- The name must clearly express the intent of the function.
  Example: input_validator(), get_png_metadata()

Struct Types:
- Use PascalCase for struct type names.
  Example: typedef struct ImageMetadata { ... } ImageMetadata;

Macros and Constants:
- Use ALL_CAPS for #define macros and global constants.
  Example: #define HEADER_LENGTH 8

---

POINTER STYLE

Always attach the * to the type, not the variable:

	int* file_ptr;

int* clearly denotes a pointer type. It reflects the fact that we are working with a different type than int. This encourages thinking in terms of pointer semantics and helps avoid confusion in mixed declarations.

---

FUNCTION GUIDELINES

- Every function must follow the single responsibility principle.
- As a rule of thumb, functions should not exceed 50 lines. 
- Each function must be documented using a comment block that describes:
	- the purpose of the function
	- parameters and their meaning
	- expected return values
	- error handling

Example:

	/**
	 * input_validator - Validates whether a file is a PNG.
	 * @input_path: The path to the file to validate.
	 *
	 * Returns: a FILE pointer if the file is valid, or NULL on failure.
	 */

---

VARIABLE DECLARATION ORDER

Variables must be declared at the top of the block in which they are used.

- Group variables with shared semantics together for clarity:
		int height, width;

- Avoid ambiguous short names in grouped declarations.

- Declare variables without initialization first, and place initialized variables after them:

		FILE* file_ptr;
		size_t read_bytes;
		unsigned char header[8] = {0};

This pattern encourages writing isolated, self-contained subroutines. It avoids the creation of long, sequential chains of stateful logic that become harder to understand and debug. Declaring initialized variables last just beacuse i am insufferable and i prefer clear text.

---

SUBROUTINE SECTIONS

When splitting a function into logical steps, use structured sub-comments to separate the sections. Each section should define its own set of local variables if needed:

	// SUB-ROUTINE: Validate input file

If necessary, use additional scoping with braces { ... } to isolate subroutine logic and reduce accidental state leakage.

---

FORMATTING AND STYLE

- Use tabs for indentation. Do not use spaces.
- Place opening braces on the same line.
- Use blank lines to separate logical blocks of code.
- Avoid mixing declarations and executable code in the same line.

---

OTHER CONVENTIONS

- Constant values that require type safety or limited scope should use const rather than #define.
- Function ordering within a file should be top-down: main logic first, followed by helper functions.
- If the codebase becomes modular, prefix functions with the module name (e.g., png_read_metadata()).

---

This style guide is designed to enforce structure, readability, and discipline in C code. When in doubt, prefer clarity over brevity.

