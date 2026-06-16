# eblenix_csgo_public
A hack for counter-strike global offensive, created solely for educational purposes in the process of learning gamehacking, which I sometimes used while playing csgo.


(See media at the [link](media/))

# How to use
Clone the repository
```
git clone https://github.com/xastrix/eblenix_csgo_public
```
Launch eblenix_csgo_public.sln, configure your visual studio settings and compile by pressing <kbd>CTRL + SHIFT + B</kbd>  
After compilation, a bin folder will appear in the root directory of the project. In that folder, run eblenix_injector.exe, then start csgo and inject
> [!NOTE]
> To build, you will need visual studio with cpp17 and the windows sdk (preferably 10.0.19041.0)

# Dependencies
- [Minhook](https://github.com/TsudaKageyu/minhook)
- [Lua 5.4](http://www.lua.org) (+sol)
- [tiny-AES-C](https://github.com/kokke/tiny-AES-C)
- [curl](https://github.com/curl/curl)