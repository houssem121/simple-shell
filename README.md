# Simple Shell Implementation

This project is a simple implementation of a Unix shell in C. It supports basic shell operations such as executing commands, input/output redirection, piping between commands, and a basic history feature.

## Features

- **Command Execution**: Execute standard Unix commands.
- **Redirection**: Supports input (`<`) and output (`>`) redirection.
- **Piping**: Allows command chaining through pipes (`|`).
- **Command History**: Basic history feature to recall the last executed command.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The project can be compiled and run on any Unix-based system. You will need `gcc` or any similar C compiler installed on your system.

### Installing

Clone the repository to your local machine:

```bash
git clone https://github.com/houssem121/simple-shell.git
```

Navigate to the cloned directory:

```bash
cd simple-shell
```

Compile the project using the included Makefile:

```bash
make
```

### Running the Shell

To start the shell, run:

```bash
./simple_shell
```

You should see the shell prompt, indicating that the shell is running:

```
osh>
```

### Using the Shell

- **Execute Commands**:
  Simply type any command and press enter.

  ```
  osh> ls -l
  ```

- **Using Redirection**:
  - Redirect output to a file:
    ```
    osh> ls -l > out.txt
    ```
  - Redirect input from a file:
    ```
    osh> sort < in.txt
    ```

- **Using Pipes**:
  ```
  osh> ls -l | less
  ```

- **Viewing Command History**:
  Type `!!` to execute the last command again.
  ```
  osh> !!
  ```



## Authors

- **Houssem Hammami** - *Initial work* - [Houssem121](https://github.com/houssem121)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.


