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
    
    bin/mac key_file message_file tag_file
    
you should get "success" messages and conversion of your hex strings to text , and promt questions loking like:

    Read key...Success
    Read message...Success
    Converted key: "Where there's love there is life"
    Converted message: "I believe that if life gives you lemons, you should make lemonade... And try to find somebody whose life has given  them vodka, and have a party."
    Select CBC-MAC or Hash-and-MAC
    Type 1 for CBC-MAC or 2 for Hash-and-MAC : 1
    Type 1 Tag Generation or type 2 for for Verification : 1
    
The output will be saved to tag if Generation is chosen. There will be a stdout with tag generation block by block:
    
    Block  0 in: 900000000c0c0c0c0c0c0c0c0c0c0c0c
    Block  0 out: d57339638a7a2805ca5c1393cb835ebc

    Block   1 in: d57339638a7a2805ca5c1393cb835ebc
    Message 1 in: 492062656c6965766520746861742069
    M XOR B 1 in: 9c535b06e6134d73af7c67fbaaf77ed5
    Block  1 out: 80497f876fb23e6888dd9ec1d5f7cc09

    Block   2 in: 80497f876fb23e6888dd9ec1d5f7cc09
    Message 2 in: 66206c69666520676976657320796f75
    M XOR B 2 in: e66913ee09d71e0fe1abfbb2f58ea37c
    Block  2 out: 9e9621d622ac5ba4d79d310ba8fd5ff0

    Block   3 in: 9e9621d622ac5ba4d79d310ba8fd5ff0
    Message 3 in: 206c656d6f6e732c20796f752073686f
    M XOR B 3 in: befa44bb4dc22888f7e45e7e888e379f
    Block  3 out: e29c6d0283f60999fe612f1978c37751

    Block   4 in: e29c6d0283f60999fe612f1978c37751
    Message 4 in: 756c64206d616b65206c656d6f6e6164
    M XOR B 4 in: 97f00922ee9762fcde0d4a7417ad1635
    Block  4 out: d5b111ec2d40de02408af62b7fcdba3c

    Block   5 in: d5b111ec2d40de02408af62b7fcdba3c
    Message 5 in: 652e2e2e20416e642074727920746f20
    M XOR B 5 in: b09f3fc20d01b06660fe84525fb9d51c
    Block  5 out: 7e51add5d4d6c5f5ff4bb7958162d090

    Block   6 in: 7e51add5d4d6c5f5ff4bb7958162d090
    Message 6 in: 66696e6420736f6d65626f6479207768
    M XOR B 6 in: 1838c3b1f4a5aa989a29d8f1f842a7f8
    Block  6 out: 9e122f5132498a139dfb9387098515d7

    Block   7 in: 9e122f5132498a139dfb9387098515d7
    Message 7 in: 6f7365206c6966652068617320676976
    M XOR B 7 in: f1614a715e20ec76bd93f2f429e27ca1
    Block  7 out: 4f83cbb1ed616648cb823388784d87d7

    Block   8 in: 4f83cbb1ed616648cb823388784d87d7
    Message 8 in: 656e207468656d20766f646b612c2061
    M XOR B 8 in: 2aedebc585040b68bded57e31961a7b6
    Block  8 out: cf1707d23402b71d19636ee14b44a8f3

    Block   9 in: cf1707d23402b71d19636ee14b44a8f3
    Message 9 in: 6e64206861766520612070617274792e
    M XOR B 9 in: a17327ba5574d23d78431e803930d1dd
    Block  9 out: 24316036ee15ea6546c8f93695bfbf76

    Block   10 in: 24316036ee15ea6546c8f93695bfbf76
    Message 10 in: 10101010101010101010101010101010
    Block  10 out: dbdc71b494c4438214a5a39716d59acc
    Tag:     dbdc71b494c4438214a5a39716d59acc

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
