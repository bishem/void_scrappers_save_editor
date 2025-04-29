Void Scrappers Save Editor
==========================
Savegame editor for [Void Scrappers](https://voidscrappers.com).

Works on OpenBSD, probably not other platforms.

For legal use only.

Quickstart
----------
1. Grab the source
   ```
   $ git clone git@github.com:magisterquis/void_scrappers_save_editor.git
   $ cd void_scrappers_save_editor
   ```
2. Build it
   ```
   $ make && make test
   ```
3. Back up the save file.  On a Mac, it's in
   `$HOME/Library/Application Support/8bitskull_void_scrappers`
   ```
   $ cp void_scrappers_save void_scrappers_save.bak
   $ scp void_scrappers_save obsd:~/void_scrappers_save_editor/
   ```
4. Print the contents of the save file, looking for the hex offset of whatever
   is to be changed.
   ```
   $ ./void_scrappers_save_editor ./void_scrappers_save
   savefile (14)
           stat_levels (8)
                   magnet_range	0x0048=231.000000
                   xp_gain	0x0060=182.000000
                   weapon_rerolls	0x007c=10.000000
                   damage	0x0090=268.000000
                   credits	0x00a8=121.000000
                   banish_upgrades	0x00c8=2.000000
                   health_regen	0x00e4=267.000000
                   cooldown	0x00fc=163.000000
           unlocked_stat_upgrades (44)
                   laser_size	0x0134=false
                   dash_effects_orbitals	0x0150=true
   ...
   ```
5. Re-run with an offset and a new value, `true` or `false` for booleans or
   a number for numbers.  E.g. to be able to banish 100 cards and increase the
   laser size...
   ```sh
   $ ./void_scrappers_save_editor ./void_scrappers_save 0x00c8=2.000000 0x0134=true
   ```
6. Put the modified file back where the game can find it.
   ```sh
   $ scp obsd:~/void_scrappers_save_editor/void_scrappers_save .
   ```

Notes
-----
1. Most of the `stat_levels` numbers increase, well, levels.  These are used
   to calculate stats values, but much like in the game hit diminishing returns
   very quickly.  65535 damage isn't much different from 268 damage.
2. I recommend playing through the game before editing the file.  It'll kill
   the fun.
3. This same code can _probably_ read the files in the same directory as the
   save file with minor modifications.
