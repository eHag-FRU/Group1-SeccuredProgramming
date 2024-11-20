# Group1-SeccuredProgramming


This project uses input sanitization (regular expressions that were a more complex white list of acceptable input), encryption with AES, authentication, as well as bounds checking and closing of streams when finished, and integrity of the log.

This allowed for user input to be given, checked to ensure it met required formatting, sanitized to ensure no injections would execute, then written to the file. Before allowing the sanitized input to be written to the file, the security key was checked against the rest of the log file (if the log file being written to already had log data in it). If the decryption failed during the checks to validate the time stamp and arrival & leaves then the key was considered valid and matching. If the validation failed by integrity violation, then the user input was not added into the log.

Librarys used:
    Crypto++  => This was for encryption and decryption done with AES.

Tools Used:
    Regular Expression => Built into C++, this allowed us to check formatting and sanitize.