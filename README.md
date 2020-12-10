# rsa-tool
Command line RSA tool written in C++, with use of the GMP library.

## Usage
1. Install the GNU MP Bignum Library.
2. Clone the git repository and navigate into its directory.
```console
git clone https://github.com/JosCla/rsa-tool && cd rsa-tool
```
3. Build the program.
```console
make
```
4. Run the program, providing usage flags to determine what the program does.
```console
./rsa [flags]
```

## Usage Flags
These flags determine what the program does, and what information it uses/outputs.

### Mode Selection
Note: These three modes are mutually exclusive, and at least one must always be used.
-g: Generate Key
  Tries to generate a full RSA key from scratch, or from what information is given through -i flags.
-e: Encrypt
	Allows the user to encrypt information.
  For this mode to function, e, n, and m must be provided through -i flags.
  Note: If no output options are specified, -oc is effectively added by default.
-d: Decrypt
  Allows the user to decrypt information.
  For this mode to function, d, n, and c must be provided through -i flags.
  Note: If no output options are specified, -om is effectively added by default.

### Input/Output Specifiers
Note: These flags can be added multiple times in a single command, in order to input from/output to multiple places.
-i=FILENAME: Input properties from FILENAME
  Inputs the properties from a given file to be used by the program.
  Properties in the given file should be of the form:
    @: 1234567890
  with @ representing the corresponding qualifier (all input/output qualifiers listed below),
  and the numbers representing the value of the qualifier.
-i*=...: Input a property manually
  Lets the user input a certain property manually.
  Putting a qualifier (all input/output qualifiers listed below) in place of the * denotes which property is being input.
	ex. -ie=65537 sets the public key to 65537
-o*: Output properties
  Lets the user output properties to standard output.
  Putting qualifiers (all input/output qualifiers listed below) in place of the * denotes which properties to output.
	ex. -oen outputs the public key and the modulus
	ex. -opqnted outputs all key properties
-o*=FILENAME: Output properties to FILENAME
  Lets the user output properties to a file.
  Putting qualifiers (all input/output qualifiers listed below) in place of the * denotes which properties to output.
	ex. -oen=pub.key -odn=priv.key outputs the public exponent and modulus to pub.key, and outputs the private exponent and modulus to priv.key
	ex. -opqnted=cool.key outputs all key properties to cool.key

### Input/Output Qualifiers
  p/q: primes used in key generation
  n: modulus
  t: totient of modulus
  e: public exponent
  d: private exponent
  m: message
  c: ciphertext
  s: message input/output in string form (converts to/from decimal to ASCII before inputting/outputting)
