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


# Assignment 3 ( description for assignmnet 2 was moved below )
## Part 1. Signature gen

To run signature gen type this:
       
     ./bin/sig -s [Private (or public) key_file][message_to_sign_file][signature_file]

There is no std output here.

To verify signature type same command with another flag:

     ./bin/sig -v [Public (or private) key_file][message_to_sign_file][signature_file]
     
There is going to be output:
    
        Signature Verified
        
   or
   
        Signature did not verify
       
#### !!! If same key is used for both, program will segfault on verifying part

## Part 2. Certificate/Private Key Generating
This part is very similar to key gen from previous assignment. Except, we modified the way public and secret keys look. Also there is signature added for Public key to make it a certificate. We were allowed to have certificate chain of size 1, thus, we have pre-generated folder with CA files that we use to sign keys for Locking and Unlocking Parties.
This is how you would run this part:
    
    ./bin/rsa 
    
 This is the resulting promt and output during execution, we used KeyGenPart folder to store generated files:
 
          Choose a task: 
        1  -  Key Generation
        2  -  RSA Encryption
        3  -  RSA Decryption

            1                
        Enter path to Public Key File
            KeyGenPart/PK.txt
        Enter path to Secret Key File
            KeyGenPart/SK.txt 
        Enter your identity
            Alice
        Is it self signed? 1(yes), 0(no)
            0

        Enter security parameter n: 
            512
        Generating p...
        Prime p is found...
        Generating q...
        Prime q is found...
        e > 1 is chosen...
        d is calculated...
        Writing Public Key to a file...
        Writing Secret Key to a file...
        Signing sertificate...
        Enter signature file: 
            KeyGenPart/Sig.txt

(If 1 is chosen for signing then PK will be signed with Secret Key that was just generated)
This is how new Public Key looks (KeyGenPart/PK.txt):

        Identity:
        Alice

        --- BEGIN PUBLIC KEY ---
        N:  
        BA9FCA4A473E1D3379597C0A3A4C19219D46BFF16AFFB09A68EE8B6CEE71E3B8E43A1E9667FFB11705FB40432132A51007E24DF3AB3D4C75EFBF6BBF1D38567CD63E6C77381333A9103272E87C685D8753E6A536472FA0584341BDEA3586D63FA9024E240668CE1329F7F084FBBCE18510FC41603EB29F4589E2637218793139
        Bits:
        200
        Key:
        10001
        ---- END PUBLIC KEY ----
        
This is Private Key (KeyGenPart/SK.txt):

        --- BEGIN SECRET KEY ---
        N:  
        BA9FCA4A473E1D3379597C0A3A4C19219D46BFF16AFFB09A68EE8B6CEE71E3B8E43A1E9667FFB11705FB40432132A51007E24DF3AB3D4C75EFBF6BBF1D38567CD63E6C77381333A9103272E87C685D8753E6A536472FA0584341BDEA3586D63FA9024E240668CE1329F7F084FBBCE18510FC41603EB29F4589E2637218793139
        Bits:
        200
        Key: 
        38D98C49EC41E3AB568007E525B520C2B8A260BE2E160DBA3BD4CEEC3406C5940112235735FF11F72D7F48282AE8BE978F8F44A7C1B63EA07E94154276A83ECFEAFF7F08251CEECE9F366E03BF57CA8F36D8BEA2BBC3FBD9F8A996373511707C7BCE2A3D1E43F4B943335F4B41C4DB3361375421627D378FD270052F2C0B4E01
        ---- END SECRET KEY ----
        
Signature file has only signature in hex ( KeyGenPart/Sig.txt ):
               
 
            67F8DF023C7AAE75E97919EEC51D12D0070106FC4091BAA4875F912C59925BE6781B42C5CDB0513EEADC74122B459AB51305EC7ECFF9FE5843BF8D8E0CB93B781A440AB677A39FFA8054ACC2446B698178C0719F3A58383729F740FCB7BD5BA668310FF6D480785451EC9BC8DE3FA13263D4AB27DB81B0AA3871416CC37E02DF671124B79C518A2134633BE07326C16FAF14390BA9F50144E88B6BBA94598F2FD021EE2D409E50DADEAF6F5A69786D0996E81F56F88B0AE5167B71A38617005A1E0656A7405912DE72787EF276FBF8FE772FE5F50C54FD7682AD5274F5966F6606910C73FA9393452EF0575222AE4681A240DFD6DDF9DA6A1E82404CDA08B219
        
## Part 3. Certificate/Private Key Generating

### !!! Please use one word for identity, there is a liitle problem with the way we read from file if identity consists of more words.

### !!! We have input2 and unlockInput2 files with pathes for input, so you can uncomment [line 18](https://github.com/pendgaft/cosc483-pa23-27/blob/master/src/LockDriver.cpp#L18) and [line 48](https://github.com/pendgaft/cosc483-pa23-27/blob/master/src/LockDriver.cpp#L48) for automated run.

For this part we pre-generated these folders:

    LockingPartyFiles/
        LockPK.txt  LockSK.txt  LockSig.txt
        
    UnlockingPartyFiles/
        UnlockPK.txt  UnlockSK.txt  UnlockSig.txt
        
    CAfiles/
        CApk.txt  CAsig.txt CAsk.txt
        
We also created a folder to encrypt that has following files (with hex encode strings of real text, you can paste it to hexToStr converter online and check):
    
    DorectoryToLock/
        a.txt b.txt c.txt

### This is how we run part a):

    ./bin/lock
    
The resulting promt and output is:

    Do you lock or unlock directory? Type 1 (lock) or 0 (unlock)
    1
    Enter path for directory to lock
        /DirectoryToLock
    Enter path for locking party Public Key
        /LockingPartyFiles/LockPK.txt
    Enter path for locking party Secret Key
        /LockingPartyFiles/LockSK.txt
    Enter path for locking party signature
        /LockingPartyFiles/LockSig.txt
    Enter path for unlocking party Public Key
        /UnlockingPartyFiles/UnlockPK.txt
    Enter path for unlocking party signature
        /UnlockingPartyFiles/UnlockSig.txt
    Enter path for CA's Public Key
        /CAfiles/CApk.txt
    Enter path for CA's signature
        /CAfiles/CAsig.txt

    Verifying CA...
    Verifying requesting party...
    Verifying locking party...
    All parties verified successfully...
    
    Generating 'SharedKeys'...
    Encrypting 'SharedKeys'...
    Signing 'SharedKeys.enc'...

    Encryptitng 'a.txt' ...
    Generating tag...
    Encryptitng 'b.txt' ...
    Generating tag...
    Encryptitng 'c.txt' ...
    Generating tag...
        
DirectoryToLock looks this way now:

    DirectoryToLock/
        CApk.txt           LockPK.txt         SharedKeys.enc     a.txt.enc          b.txt.enc          c.txt.enc
        CAsig.txt          LockSig.txt        SharedKeys.enc.sig a.txt.tag          b.txt.tag          c.txt.tag
        
### This is how we run part b): 

Now we run the same command for unlocking, but we choose 0 on promt to unlock the same folder

    Do you lock or unlock directory? Type 1 (lock) or 0 (unlock)
        0
    Enter path for directory to unlock
        /DirectoryToLock
    Enter path for unlocking party Public Key
        /UnlockingPartyFiles/UnlockPK.txt
    Enter path for unlocking party Secret Key
        /UnlockingPartyFiles/UnlockSK.txt
    Enter path for unlocking party signature
        /UnlockingPartyFiles/UnlockSig.txt

    Verifying CA...Verifying unlocking party...
    Verifying locking party...
    All parties verified successfully...

    Locker party identity is...Alice
    CA identity is...CAuthority

    SharedKeys.enc is verified...
    Shared Keys read...
    
    a.txt.tag and a.txt.enc are verified
    decrypt a.txt
    b.txt.tag and b.txt.enc are verified
    decrypt b.txt
    c.txt.tag and c.txt.enc are verified
    decrypt c.txt
    
Resulting directory again is:
    
    DorectoryToLock/
        a.txt b.txt c.txt
        
### !!! The only differens in file contents is that they are upper case hex strings now, but it doesn't change their values. When we feed them to hexToStr converter we get the same text strings back.

### !!! If files get altereted decryption will not work, there might be an error message, for example if identity on public key is changed, if key is altered itself program can simply crush...sorry

------------------------------------------------------------------------------------------------------------------------------------------
# Assignment 2 

AES can be ran with

    bin/aes [MODE] [OPTION] [KEY_FILE] [IMPUT_FILE] [OUTPUT_FILE]
An example mignt be

    bin/aes -cbc -e files/key.txt files/message.txt files/cypher.txt
    bin/aes -cbc -d files/key.txt files/cypher.txt files/message2.txt
    
These two commands would encrypt `-e` the message `files/message.txt` 
using CBC `-cbc` using the key in file `files/key.txt` and placing the cypher in `files/cypher.txt`.  Decrypting would be similar only using the `-d` flag and the input file this time should be the cypher file `files/cypher.txt` and the output file will be the resulting message `files/message2.txt`.  YOu can get a help message with a discription of all the flags and modes using the `--help` flag.

------

MAC can be run with
    
    bin/mac [key_file] [message_file] [tag_file]
    
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
