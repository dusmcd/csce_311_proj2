# Build and Execution Instructions

- In the `proj2` directory run the `make` command
- This will create two executables: *bool-expr-server* and *bool-expr-client*
- Run the server, providing the following arguments (in order):
  - A path to a text file containing boolean expressions (example provided in `dat/` directory)
  - The name of the socket
  - The unit-separator character
  - The end-of-transmission character
- Once the server is running, run the client, providing the following arguments (in order):
  - The name of the socket (**must be the same name as the server socket**)
  - Boolean values for variables a, b, c, etc. in order separated by a space
    (e.g., T F T => a = True, b = False, c = True)

# File Structure
proj2/
+-- dat/
|   +-- expr_10.txt
|   +-- expr_25k.txt
|
+-- include/
|   +-- bool_expr_client.h
|   +-- bool_expr_server.h
|
+-- src/
|   +-- bool_expr_client.cc
|   +-- bool_expr_server.cc
|
+-- Makefile
+-- README.md