# eblenix_csgo_public
A hack for counter-strike global offensive, created solely for educational purposes in the process of learning gamehacking, which i sometimes used while playing csgo
## Note
This cheat was created for educational purposes, it has typical cheat functions for csgo, some parts of protection and string/import protection (xorstr, lazy) were removed from the code.  
There are some issues in the code, but there was not a single crash either while using the cheat or when unloading it.  
Below is a list of csgo versions on which this cheat works; unfortunately, I don't have time to update it to the latest game version and don't see the point in doing so.  
```
1.37.3.1, 1.37.3.8
```
## Injector
The injector has two functions, loading and unloading cheat, unloading works by intercepting messages (WM_COPYDATA)
## Using
Visual Studio is required for compilation, i am using the 2017 version, compile eblenix_csgo & eblenix_injector and open the injector, then inject it into the game.  
The menu is navigated with the arrow keys, and it can be opened by pressing the ```INSERT``` key.
### Some info
There are errors in the code; if I have free time, I will try to fix them. Also, the cheat does not have a skin changer because when I used this cheat, I did not need a skin changer, so I considered it unnecessary.