# LogicGateSim
Logic gate simulator for c project.

# Usage
## Commands over cli
- `run [inputFile]`

   Imports gates from file and runs any provided inputs through these gates. The result of each simulation is then shown.

- `rs [inputFile] [outputFile]`

   Imports gates from `inputFile` and outputs the results to `outputFile`.

- `quit`

   Exits the program.

# Things remaining to be done
- Change method of adding inputs so that they are ordered
- Verify data integrity (before processing)
- Check for unsused gates
- Add relevant error messages for user to help clear up mistakes
- Remove debugging text!!
- Change folder structure
- Set up CMakeLists.txt
- Set up unit testing
