# cosc483-pa23-27

## Compiling

To compile use

    make
or 

    make all
To clean the directory use

    make clean
To compile program individually use

    make bin/aes
    make bin/rsa
    make bin/mac
## Directory layout.

All programs are found in the `bin/` folder.

All object files are found in the `obj/` folder.

All header file are found in the `include/` folder.

All supporting files are found in the `files/` folder...

## Programs
AES can be ran with

    bin/aes [MODE] [OPTION] [KEY_FILE] [IMPUT_FILE] [OUTPUT_FILE]
An example mignt be

    bin/aes -cbc -e files/key.txt files/message.txt files/cypher.txt
    bin/aes -cbc -d files/key.txt files/cypher.txt files/message2.txt
These two commands would encrypt `-e` the message `files/message.txt` using CBC `-cbc` using the key in file `files/key.txt` and placing the cypher in `files/cypher.txt`.  Decrypting would be similar only using the `-d` flag and the input file this time should be the cypher file `files/cypher.txt` and the output file will be the resulting message `files/message2.txt`.  YOu can get a help message with a discription of all the flags and modes using the `--help` flag.

------

MAC can be ran with

-----

RSA can be ran with

    bin/rsa
you should get a menue that looks like:

    1  -  Key Generation
    2  -  RSA Encryption
    3  -  RSA Decryption
These should all be self explainatory.  Selecting Key Generation you should be prompted for the path and file name to the public key:

    1
    Enter path to Public Key File
    files/PK.txt
The you will be asked for the path and file name to the secret key:

    Enter path to Secret Key File
    files/SK.txt
Next you will be asked for a security parameter:

    Enter security parameter n: 
    256
If everything went well you should see something like the following:

    Generating p...
    Prime p is found...
    p: 59230156565197750660218095073857452292400856064621825406706676254984406683193
    
    Generating q... 
    Prime q is found...
    q: 68554868690979838725520867908778912648178648501034720795587043617536339225841
    
    e > 1 is chosen...
    d is calculated...
    e: 65537
    order of the Group: 4060515605873309221515857145716771636277505000541357643700104848586305443820416751478464966208588331165860280556671436145610501642097851907198919420081280
    d:    367346644295937415114630163372670995490933780127404511489661132600946106419446281024700837460529475189318790964195873245759260635000048277427749107247233
    Writing Public Key to a file...
    Writing Secret Key to a file...
    
When you run RSA Encryption or RSA Decryption you will be instructed for the Enter path to Public Key File, path to Plain Text File, and the path to Cipher Text File. It should look something like this:

    Enter path to Public Key File
    files/PK.txt
    Enter path to Plain Text File
    files/message.txt
    Enter path to Cipher Text File
    files/cypher.txt
If you are returned to your command line prompt, everyting went as planed.  There should be no other output.

    phillip:project$
