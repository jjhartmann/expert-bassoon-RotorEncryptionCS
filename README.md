# expert-bassoon-RotorEncryptionCS
Basic client server with rotor encryption simulation.

## Rotor Encryptioin Machine Simulation
The simulation for the rotor machine is housed inside the class `class RotorEncryption` and is composed of two parts. 
* Part 1 sees the creation of the encryptionscheme.txt file.
* Part 2 uses the enccyrptionscheme.txt to build the different schemes. 

### 1. Encryption Scheme Creation
The static mehthod: 
```C++ 
void RotorEncryption::buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount) 
```
Takes in two variables, the number of rotors per machine and the number of schemes to create. 
From here, the method will generate a random permutation of the rotors by using a map of ascii characters. 

```
 !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~
 ```
 For each scheme, the permuatation of the string are printed out to the .txt file. 
 
 
### 2. Build Rotor Machine
 Upon initalization, the program will read from the encryptionscheme.txt file to produce a map of input and outputs for each rotor. 
 Each scheme is represented by the struct `EScheme` that is delcared in the header file. 
```C++
// Struct definition for Encruyption scheme
struct EScheme {
    // ... (code)
    int schemeId;
    int rotorCount;
    char *ioMapEn;
    char *ioMapDe;
};
```
This will encapulate the map for encryption (`ioMapEn`) and decryptoin (`ioMapDe`) from the ascie input to the permuted output. 

Upon intializatin of the rotorencryption object, a schemeId will be chosen to specify which of the rotormachines to use.
The offset for each of the individual rotors will be mainted by the `int *offsetMap;`. 


## Diffie-Hellman Algorithm
The diffie-hellman algorithm is housed inside the class `DiffieHellman`. 
The following are the steps the algorithm takes:

1. The server will determine the what the intial two public keys will be (G, P) and send them to the client. 
2. Both the server and client will choose a random private key (a, b). 
3. Both will generate another public key by performing the operation: `pow(G, privateKey) mod P`. 
4. Both will exchange these generated public keys A and B with eachother. 
5. Finally, both will generate the shared key by performing:
  * Server: `pow(G, ClientB) mod P`
  * Client: `pow(G, ServerA) mod P`

The server will then choose a schemeId to use with the rotor machine, and will send that ID by encrypting it with the shared key.
The client will then decrypt the schemeId and both will communicate witht the specified rotor scheme. 

## Data Samples

### Sample 1
From the Client:
```
   There is neither happiness nor unhappiness in this world; there is only the comparison of one state with another.
   Only a man who has felt ultimate despair is capable of feeling ultimate bliss. It is necessary to have wished for
   death in order to know how good it is to live.....the sum of all human wisdom will be contained in these two words: 
   Wait and Hope.

   Encrypted Text: ;_>5=]1'3U'LT'j%^jT+DpV<A/XgqhqB,~.r[s5Lzn1ud"FnOP[M-LThFpY!y$fU]VH6tk7CNT(x3u_k"xh.,#,}N}5[]k>Cz
   sLEyuTFqPRQOhQamwA@d8#2lDBe[kou-N:!`7IjFU3X|Z$dE.ijR(J@AfK~}I]e_|9['JhY.<ZEQ\fbSLvPU.1!~^#2Ct`'2d>BivK/!2HCich47E
   uk-x%ud5.)?:`>2.N8!k5L29 ih@$x%`YQ'mQOjY{kX+;$q<.l;Br#{n(;$"zH;]_S3%.Xry$*N>Z,0#*rFR&!zrho,:];sqLkHcA]'~.bycs>itE*
    BzB{Ae{pIxs&TVzNq>yQ]{Rs%S-B[5;?;
```
Form the Server:
```
   Received encrypted message: ;_>5=]1'3U'LT'j%^jT+DpV<A/XgqhqB,~.r[s5Lzn1ud"FnOP[M-LThFpY!y$fU]VH6tk7CNT(x3u_k"xh.,#,}N}5[]k>Cz
   sLEyuTFqPRQOhQamwA@d8#2lDBe[kou-N:!`7IjFU3X|Z$dE.ijR(J@AfK~}I]e_|9['JhY.<ZEQ\fbSLvPU.1!~^#2Ct`'2d>BivK/!2HCich47E
   uk-x%ud5.)?:`>2.N8!k5L29 ih@$x%`YQ'mQOjY{kX+;$q<.l;Br#{n(;$"zH;]_S3%.Xry$*N>Z,0#*rFR&!zrho,:];sqLkHcA]'~.bycs>itE*
    BzB{Ae{pIxs&TVzNq>yQ]{Rs%S-B[5;?;

   Decrypted message: There is neither happiness nor unhappiness in this world; there is only the comparison of one state
   with another. Only a man who has felt ultimate despair is capable of feeling ultimate bliss. It is necessary to have 
   wished for death in order to know how good it is to live.....the sum of all human wisdom will be contained in these two
   words: Wait and Hope.
```

### Sample 2

From Client:
```
  Whenever I find myself growing grim about the mouth; whenever it is a damp, drizzly November in my soul; whenever I 
  find myself   involuntarily pausing before coffin warehouses, and bringing up the rear of every funeral I meet; and
  especially when my hypos get such an upper hand of me, that it requires a strong moral principle to prevent me from 
  deliberately stepping into the street, and methodically knocking people's hats off--then, I account it high time to
  get to sea as soon as I can.

  Encrypted Text: X_>p=g8435>&*cB w{Yt#Cje'tUU(EqtX6pA4Wo:@;0*n"6r.9$=n7QU vwc-&dD!_Bc+6~/G)Wx#Ce'e>SI@)cG^\d:5Db|%}Zm
  nHOWqs9\W`}PYw9;sG8}lDe\s=Ib@E8Iedq^J-|X{cId8TYNXx3]8>C%(W |j#`}{EI|(Xo%X@;_/sb>~dvu?y(veQss3A`B0&P^(fn:yGfZRJPCrB<B)Wa_
  2:,wS=M!!^.W9R=^d08+t|O=gm];AJ#wXX;$Es"mEw%[<Sbm\pwMQ9;FF!nDcozNCA71~8V;F]G]UI>]OXpaM$'"PhAK[E6s;dq>i_0%(<zBH-^N~uo)
  ,?="v"-g'frb@o#p!0%&e^I* c*r$jK5QXE8RaIg_H+JtaBfH}63fG{,Z| |1_!v-I8,mz'+Jf5SQ;>gUOb_nqj`@bm+RdqUu|Xj+ZL:,6S<#miLAgVF
  Oo-gc%"QM*a !nHG$%jKQ(8*'lL6b)Z\e`.kT\
```

From Server:
```
  Received encrypted message: X_>p=g8435>&*cB w{Yt#Cje'tUU(EqtX6pA4Wo:@;0*n"6r.9$=n7QU vwc-&dD!_Bc+6~/G)Wx#Ce'e>SI@)cG^\d:5Db|%}Zm
  nHOWqs9\W`}PYw9;sG8}lDe\s=Ib@E8Iedq^J-|X{cId8TYNXx3]8>C%(W |j#`}{EI|(Xo%X@;_/sb>~dvu?y(veQss3A`B0&P^(fn:yGfZRJPCrB<B)Wa_
  2:,wS=M!!^.W9R=^d08+t|O=gm];AJ#wXX;$Es"mEw%[<Sbm\pwMQ9;FF!nDcozNCA71~8V;F]G]UI>]OXpaM$'"PhAK[E6s;dq>i_0%(<zBH-^N~uo)
  ,?="v"-g'frb@o#p!0%&e^I* c*r$jK5QXE8RaIg_H+JtaBfH}63fG{,Z| |1_!v-I8,mz'+Jf5SQ;>gUOb_nqj`@bm+RdqUu|Xj+ZL:,6S<#miLAgVF
  Oo-gc%"QM*a !nHG$%jKQ(8*'lL6b)Z\e`.kT\

Decrypted message: Whenever I find myself growing grim about the mouth; whenever it is a damp, drizzly November in my soul; 
whenever I find myself involuntarily pausing before coffin warehouses, and bringing up the rear of every funeral I meet; 
and especially when my hypos get such an upper hand of me, that it requires a strong moral principle to prevent me from
deliberately stepping into the street, and methodically knocking people's hats off--then, I account it high time to get 
to sea as soon as I can.
```
