#include "cross_lib.h"

uint8_t fo, in_, de, fr, ro, fa, rf, at, tk;
uint16_t dn;
uint8_t ks, kf, kt, ka, wx;
uint8_t fu, nm, am, zz;
uint8_t su, gw, ee, qq;

void marrakesch(void);

uint8_t d21_roll(void) {
    return _XL_RAND() % 21 + 1;
}

uint8_t d6_roll(void) {
    return _XL_RAND() % 6 + 1;
}

uint8_t d6_sum(void) {
    return d6_roll() + d6_roll();
}

void wait_key(void) {
    _XL_PRINT(10, 22, "PRESS ANY KEY TO CONTINUE");
    _XL_WAIT_FOR_INPUT();
}

void show_status(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 0, "CHARACTER STATUS");
    _XL_PRINT(2, 2, "FORCE");
    _XL_PRINTD(10, 2, 2, fo);
    _XL_PRINT(14, 2, "INTELLIGENCE");
    _XL_PRINTD(28, 2, 2, in_);
    _XL_PRINT(2, 3, "DEXTERITY");
    _XL_PRINTD(12, 3, 2, de);
    _XL_PRINT(16, 3, "FORTUNE");
    _XL_PRINTD(26, 3, 2, fr);
    _XL_PRINT(2, 4, "ROBUSTNESS");
    _XL_PRINTD(14, 4, 2, ro);
    _XL_PRINT(2, 5, "CURRENT HP");
    _XL_PRINTD(14, 5, 2, rf);
    _XL_PRINT(2, 6, "CHARM");
    _XL_PRINTD(10, 6, 2, fa);
    _XL_PRINT(2, 8, "COMBAT ABILITY");
    _XL_PRINTD(18, 8, 3, at);
    _XL_PRINT(22, 8, "PCT");
    _XL_PRINT(2, 10, "MONEY");
    _XL_PRINTD(10, 10, 5, dn);
    _XL_PRINT(2, 12, "WEEK");
    _XL_PRINTD(8, 12, 2, tk);
    _XL_PRINT(2, 14, "ITEMS:");
    if (kf > 0) { _XL_PRINT(10, 14, "FILTER"); }
    if (kt > 0) { _XL_PRINT(18, 14, "TALISMAN"); }
    if (ka > 0) { _XL_PRINT(28, 14, "AMULET"); }
    if (ks > 0) { _XL_PRINT(36, 14, "SWORD"); }
    if (kf == 0 && kt == 0 && ka == 0 && ks == 0) {
        _XL_PRINT(10, 14, "NONE");
    }
    wait_key();
}

void clamp_stats(void) {
    ee = 0;
    if (fo > 19) { ee = ee + 1; fo = 20; }
    if (in_ > 19) { ee = ee + 1; in_ = 20; }
    if (de > 19) { ee = ee + 1; de = 20; }
    if (fr > 19) { ee = ee + 1; fr = 20; }
    if (ro > 19) { ee = ee + 1; ro = 20; }
    if (fa > 19) { ee = ee + 1; fa = 20; }
    if (rf > ro) rf = ro;
}

uint8_t get_choice(uint8_t max) {
    uint8_t inp, n;
    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(2, 20, "ENTER CHOICE (1-");
    _XL_PRINTD(18, 20, 1, max);
    _XL_PRINT(19, 20, ")");
    // _XL_WAIT_FOR_INPUT();
    inp = _XL_INPUT();
    if (_XL_LEFT(inp)) n = 1;
    else if (_XL_RIGHT(inp)) n = 2;
    else if (_XL_UP(inp)) n = 3;
    else if (_XL_DOWN(inp)) n = 4;
    else if (_XL_FIRE(inp)) n = 5;
    else n = 0;
    while (n == 0 || n > max) {
        _XL_PRINT(2, 21, "INVALID. TRY AGAIN.");
        // _XL_WAIT_FOR_INPUT();
        inp = _XL_INPUT();
        if (_XL_LEFT(inp)) n = 1;
        else if (_XL_RIGHT(inp)) n = 2;
        else if (_XL_UP(inp)) n = 3;
        else if (_XL_DOWN(inp)) n = 4;
        else if (_XL_FIRE(inp)) n = 5;
        else n = 0;
    }
    return n;
}

void title_screen(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(10, 2, "LA SYSTEMS EDITORIALE");
    _XL_PRINT(16, 4, "PRESENTS");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(20, 7, "C");
    _XL_PRINT(22, 7, "O");
    _XL_PRINT(24, 7, "N");
    _XL_PRINT(26, 7, "A");
    _XL_PRINT(28, 7, "N");
    _XL_PRINT(16, 10, "A HEROIC ADVENTURE");
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(10, 14, "PRESS ANY KEY TO BEGIN");
    _XL_WAIT_FOR_INPUT();
}

void char_select(void) {
    uint8_t n, ch;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(4, 0, "CHOOSE YOUR CHARACTER");
    _XL_PRINT(2, 3, "1: SWENHA THE APPRENTICE MAGE");
    _XL_PRINT(4, 4, "FO 9 IN 18 DE 11 FR 14 RO 11 FA 17");
    _XL_PRINT(2, 6, "2: THRULKH THE WARRIOR");
    _XL_PRINT(4, 7, "FO 17 IN 10 DE 13 FR 10 RO 16 FA 9");
    _XL_PRINT(2, 9, "3: COLVOR THE ADVENTURER");
    _XL_PRINT(4, 10, "FO 12 IN 14 DE 16 FR 18 RO 12 FA 12");
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, 12, "4: RANDOM CHARACTER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    n = get_choice(4);
    ch = n;
    if (ch == 1) {
        fo = 9; in_ = 18; de = 11; fr = 14; ro = 11; fa = 17;
    } else if (ch == 2) {
        fo = 17; in_ = 10; de = 13; fr = 10; ro = 16; fa = 9;
    } else if (ch == 3) {
        fo = 12; in_ = 14; de = 16; fr = 18; ro = 12; fa = 12;
    } else {
        fo = d6_sum() + 6;
        in_ = d6_sum() + 6;
        de = d6_sum() + 6;
        fr = d6_sum() + 6;
        ro = d6_sum() + 6;
        fa = d6_sum() + 6;
        clamp_stats();
    }
    at = fo * 5;
    rf = ro;
    dn = 500;
    tk = 1;
    ks = 0; kf = 0; kt = 0; ka = 0; wx = 0;
    fu = 0; qq = 0; gw = 0; su = 0;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(4, 2, "YOUR CHARACTER IS CREATED!");
    _XL_PRINT(4, 4, "PRESS ANY KEY TO CONTINUE");
    _XL_WAIT_FOR_INPUT();
}

void combat(uint8_t enemies, uint8_t enemy_atk) {
    uint8_t turn, j, dmg, pa, cc;
    uint8_t eroll, edmg;
    uint8_t alive;
    nm = enemies;
    am = enemy_atk;
    alive = 1;
    turn = 1;
    while (alive && nm > 0) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 0, "COMBAT - TURN");
        _XL_PRINTD(16, 0, 2, turn);
        _XL_PRINT(2, 2, "ENEMIES LEFT:");
        _XL_PRINTD(14, 2, 2, nm);
        _XL_PRINT(2, 3, "YOUR HP:");
        _XL_PRINTD(10, 3, 2, rf);
        _XL_PRINT(14, 3, "COMBAT:");
        _XL_PRINTD(22, 3, 3, at);
        _XL_PRINT(26, 3, "PCT");
        for (j = 0; j < nm && alive; j++) {
            eroll = _XL_RAND() % 100 + 1;
            if (eroll <= am) {
                if (ks > 0) {
                    cc = at / 20;
                    if (cc < 1) cc = 1;
                    if (_XL_RAND() % 100 < cc) {
                        _XL_SET_TEXT_COLOR(_XL_GREEN);
                        _XL_PRINT(2, 5, "BLOCKED!");
                        continue;
                    }
                }
                edmg = _XL_RAND() % 4 + 1;
                if (edmg > rf) edmg = rf;
                rf = rf - edmg;
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(2, 6, "HIT! -");
                _XL_PRINTD(10, 6, 1, edmg);
                _XL_PRINT(12, 6, "HP");
                if (rf <= 0) { rf = 0; alive = 0; }
            }
        }
        if (rf <= 0) break;
        {
            uint8_t roll;
            roll = _XL_RAND() % 100 + 1;
            if (roll <= at || at > 100) {
                if (nm >= 4) {
                    pa = at / 4;
                } else {
                    pa = at / nm + 10;
                }
                if (pa > 95) pa = 95;
                eroll = _XL_RAND() % 100 + 1;
                if (eroll <= pa || pa > 95) {
                    dmg = _XL_RAND() % 5 + 3;
                    _XL_SET_TEXT_COLOR(_XL_GREEN);
                    _XL_PRINT(2, 8, "YOU HIT! -");
                    _XL_PRINTD(12, 8, 1, dmg);
                    nm = nm - 1;
                    if (nm == 0) {
                        _XL_PRINT(2, 10, "ALL ENEMIES SLAIN!");
                        at = at + 3;
                    }
                } else {
                    _XL_SET_TEXT_COLOR(_XL_YELLOW);
                    _XL_PRINT(2, 8, "ENEMY DODGED!");
                }
            } else {
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(2, 8, "YOU MISSED!");
            }
        }
        if (nm > 0 && rf > 0) {
            uint8_t c, fr_road;
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(2, 12, "1: FIGHT  2: FLEE");
            c = get_choice(2);
            if (c == 2) {
                fr_road = d21_roll();
                if (fr_road <= fr) {
                    _XL_CLEAR_SCREEN();
                    _XL_SET_TEXT_COLOR(_XL_YELLOW);
                    _XL_PRINT(4, 4, "YOU FLED SUCCESSFULLY!");
                    fu = 1;
                    if (fa > 0) fa = fa - 1;
                    wait_key();
                    alive = 0;
                    nm = 0;
                } else {
                    uint8_t fdmg;
                    _XL_CLEAR_SCREEN();
                    _XL_SET_TEXT_COLOR(_XL_RED);
                    _XL_PRINT(4, 4, "FLEE FAILED! TAKING DAMAGE");
                    fdmg = nm;
                    if (fdmg > rf) fdmg = rf;
                    rf = rf - fdmg;
                    wait_key();
                }
            }
        }
        turn = turn + 1;
    }
    if (rf <= 0) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(10, 8, "YOU DIED!");
        _XL_PING_SOUND();
        wait_key();
    }
    fu = 0;
}

void boss_fight(uint8_t boss_atk, uint8_t boss_max_hp) {
    uint8_t bhp, turn, dmg, cc;
    uint8_t eroll, edmg;
    bhp = boss_max_hp;
    turn = 1;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(4, 2, "BOSS FIGHT!");
    wait_key();
    while (bhp > 0 && rf > 0) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 0, "BOSS BATTLE - TURN");
        _XL_PRINTD(18, 0, 2, turn);
        _XL_PRINT(2, 2, "BOSS HP:");
        _XL_PRINTD(10, 2, 2, bhp);
        _XL_PRINT(14, 2, "YOUR HP:");
        _XL_PRINTD(22, 2, 2, rf);
        eroll = _XL_RAND() % 100 + 1;
        if (eroll <= boss_atk || boss_atk > 95) {
            if (ks > 0) {
                cc = at / 20;
                if (cc < 1) cc = 1;
                if (_XL_RAND() % 100 < cc) {
                    _XL_SET_TEXT_COLOR(_XL_GREEN);
                    _XL_PRINT(2, 4, "BLOCKED!");
                    goto boss_player_turn;
                }
            }
            edmg = _XL_RAND() % 5 + 2;
            if (edmg > rf) edmg = rf;
            rf = rf - edmg;
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(2, 5, "BOSS HITS! -");
            _XL_PRINTD(14, 5, 1, edmg);
        } else {
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT(2, 5, "BOSS MISSED!");
        }
        if (rf <= 0) { rf = 0; break; }
        boss_player_turn:
        {
            uint8_t roll;
            roll = _XL_RAND() % 100 + 1;
            if (roll <= at || at > 100) {
                dmg = _XL_RAND() % 5 + 3;
                if (dmg > bhp) dmg = bhp;
                bhp = bhp - dmg;
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(2, 7, "YOU HIT BOSS! -");
                _XL_PRINTD(16, 7, 1, dmg);
                if (bhp <= 0) {
                    _XL_PRINT(2, 9, "BOSS SLAIN!");
                    at = at + 7;
                }
            } else {
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(2, 7, "YOU MISSED!");
            }
        }
        turn = turn + 1;
    }
    if (rf <= 0) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(10, 8, "YOU DIED!");
        _XL_PING_SOUND();
        wait_key();
    }
}

void random_encounter(void) {
    uint8_t ecount;
    uint8_t eatk;
    ecount = _XL_RAND() % 4 + 1;
    eatk = (_XL_RAND() % 40) + 20;
    if (eatk > 95) eatk = 95;
    combat(ecount, eatk);
}

void marrakesch(void) {
    uint8_t ch, sub;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(6, 0, "MARRAKESCH");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 3, "1: FAETAL SWORD SHOP");
    _XL_PRINT(2, 5, "2: ALCHEMIST");
    _XL_PRINT(2, 7, "3: MAGIC ITEMS");
    _XL_PRINT(2, 9, "4: BACK");
    ch = get_choice(4);
    if (ch == 4) return;
    if (ch == 1) {
        if (ks > 0) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(4, 4, "YOU ALREADY OWN A SWORD!");
            wait_key();
            return;
        }
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "FAETAL SWORD SHOP");
        _XL_PRINT(2, 4, "1: +50%  COST 7500");
        _XL_PRINT(2, 6, "2: +75%  COST 10000");
        _XL_PRINT(2, 8, "3: +100% COST 20000");
        _XL_PRINT(2, 10, "4: BACK");
        sub = get_choice(4);
        if (sub == 4) return;
        if (sub == 1) {
            if (dn >= 7500) {
                dn = dn - 7500;
                at = at + 50;
                ks = 1;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "SWORD +50% PURCHASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        } else if (sub == 2) {
            if (dn >= 10000) {
                dn = dn - 10000;
                at = at + 75;
                ks = 1;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "SWORD +75% PURCHASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        } else if (sub == 3) {
            if (dn >= 20000) {
                dn = dn - 20000;
                at = at + 100;
                ks = 1;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "SWORD +100% PURCHASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        }
    }
    if (ch == 2) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "ALCHEMIST");
        _XL_PRINT(2, 4, "1: +1 FORCE  COST 2000");
        _XL_PRINT(2, 6, "2: +1 DEXTERITY COST 1000");
        _XL_PRINT(2, 8, "3: +1 INTELL  COST 1000");
        _XL_PRINT(2, 10, "4: BACK");
        sub = get_choice(4);
        if (sub == 4) return;
        if (sub == 1) {
            if (dn >= 2000) {
                dn = dn - 2000;
                fo = fo + 1;
                at = at + 5;
                clamp_stats();
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "FORCE INCREASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        } else if (sub == 2) {
            if (dn >= 1000) {
                dn = dn - 1000;
                de = de + 1;
                clamp_stats();
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "DEXTERITY INCREASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        } else if (sub == 3) {
            if (dn >= 1000) {
                dn = dn - 1000;
                in_ = in_ + 1;
                clamp_stats();
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "INTELLIGENCE INCREASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        }
    }
    if (ch == 3) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "MAGIC ITEMS");
        _XL_PRINT(2, 4, "1: TALISMAN +1 LUCK 1000");
        _XL_PRINT(2, 6, "2: AMULET +1 INTEL 1000");
        _XL_PRINT(2, 8, "3: HEALING FILTER 1000");
        _XL_PRINT(2, 10, "4: BACK");
        sub = get_choice(4);
        if (sub == 4) return;
        if (sub == 1) {
            if (dn >= 1000) {
                dn = dn - 1000;
                kt = kt + 1;
                fr = fr + 1;
                clamp_stats();
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "TALISMAN PURCHASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        } else if (sub == 2) {
            if (dn >= 1000) {
                dn = dn - 1000;
                ka = ka + 1;
                in_ = in_ + 1;
                clamp_stats();
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "AMULET PURCHASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        } else if (sub == 3) {
            if (dn >= 1000) {
                dn = dn - 1000;
                kf = kf + 1;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "HEALING FILTER PURCHASED!");
                wait_key();
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
                wait_key();
            }
        }
    }
}

void tavern(void) {
    uint8_t ch;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(4, 0, "WEEK ");
    _XL_PRINTD(10, 0, 2, tk);
    _XL_PRINT(14, 0, "- THE TAVERN");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 3, "1: HUNT A MONSTER");
    _XL_PRINT(2, 5, "2: RESCUE MISSION");
    _XL_PRINT(2, 7, "3: CARAVAN TO MARRAKESCH");
    _XL_PRINT(2, 9, "4: BARON BURLUND MISSION");
    _XL_PRINT(2, 11, "5: REST (COSTS 100)");
    _XL_PRINT(2, 13, "6: VIEW STATUS");
    ch = get_choice(6);
    if (ch == 6) {
        show_status();
        return;
    }
    if (ch == 5) {
        if (dn >= 100) {
            if (rf >= ro) {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "ALREADY AT FULL HP!");
                wait_key();
                return;
            }
            dn = dn - 100;
            rf = rf + 1;
            tk = tk + 1;
            clamp_stats();
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(4, 4, "RESTED. HP RECOVERED 1.");
            wait_key();
            return;
        } else {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
            wait_key();
            return;
        }
    }
    if (ch == 1) {
        uint8_t batk;
        tk = tk + 1;
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "TRAVELING TO HUNT...");
        wait_key();
        if (_XL_RAND() % 10 < 3) {
            random_encounter();
            if (rf <= 0) return;
        }
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(4, 4, "YOU FOUND THE MONSTER!");
        wait_key();
        batk = (_XL_RAND() % 30) + 30;
        if (batk > 95) batk = 95;
        boss_fight(batk, _XL_RAND() % 10 + 5);
        if (rf <= 0) return;
        {
            uint16_t reward;
            reward = (_XL_RAND() % 500) + 200;
            dn = dn + reward;
            if (fa < 20) fa = fa + 1;
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(4, 3, "VICTORY! REWARD:");
            _XL_PRINTD(18, 3, 5, dn);
            wait_key();
        }
        return;
    }
    if (ch == 2) {
        uint8_t wp, batk;
        tk = tk + 2;
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "RESCUE MISSION - 2 WEEKS");
        wait_key();
        for (wp = 0; wp < 2; wp++) {
            if (_XL_RAND() % 10 < 3) {
                random_encounter();
                if (rf <= 0) return;
            }
        }
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(4, 4, "YOU ARRIVED AT THE LOCATION");
        wait_key();
        {
            uint8_t stealth;
            stealth = d21_roll();
            if (stealth <= de) {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(4, 4, "YOU SLIPPED PAST GUARDS!");
                wait_key();
                boss_fight(50, 8);
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(4, 4, "SPOTTED! SURROUNDED!");
                wait_key();
                combat(4, 40);
            }
            if (rf <= 0) return;
        }
        {
            uint16_t reward;
            reward = (_XL_RAND() % 800) + 400;
            dn = dn + reward;
            if (fa < 20) fa = fa + 1;
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(4, 3, "MISSION COMPLETE! REWARD:");
            _XL_PRINTD(24, 3, 5, dn);
            wait_key();
        }
        return;
    }
    if (ch == 3) {
        uint8_t wp, rec;
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "CARAVAN TO MARRAKESCH");
        _XL_PRINT(2, 4, "COST: 1000 (ROUND TRIP)");
        wait_key();
        if (dn < 1000) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(4, 4, "NOT ENOUGH MONEY!");
            wait_key();
            return;
        }
        dn = dn - 1000;
        tk = tk + 4;
        for (wp = 0; wp < 4; wp++) {
            if (_XL_RAND() % 10 < 2) {
                random_encounter();
                if (rf <= 0) return;
            }
        }
        rec = ro - rf;
        if (rec > 4) rec = 4;
        if (rec > 0) rf = rf + rec;
        marrakesch();
        tk = tk + 4;
        for (wp = 0; wp < 4; wp++) {
            if (_XL_RAND() % 10 < 1) {
                random_encounter();
                if (rf <= 0) return;
            }
        }
        rec = ro - rf;
        if (rec > 4) rec = 4;
        if (rec > 0) rf = rf + rec;
        return;
    }
    if (ch == 4) {
        uint8_t wp, mt;
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 2, "BARON BURLUND RECRUITS YOU");
        _XL_PRINT(2, 4, "4 WEEKS OF SERVICE");
        _XL_PRINT(2, 6, "PAY: 1000 UPFRONT");
        wait_key();
        dn = dn + 1000;
        tk = tk + 4;
        for (wp = 0; wp < 4; wp++) {
            uint16_t loot;
            mt = _XL_RAND() % 5;
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(2, 1, "WEEK");
            _XL_PRINTD(6, 1, 1, wp + 1);
            _XL_PRINT(2, 3, "MISSION:");
            if (mt == 0) {
                _XL_PRINT(10, 3, "PATROL AGAINST MONGOLS");
                combat(_XL_RAND() % 3 + 2, 35);
            } else if (mt == 1) {
                _XL_PRINT(10, 3, "DESTROY BANDITS");
                combat(_XL_RAND() % 4 + 2, 30);
            } else if (mt == 2) {
                _XL_PRINT(10, 3, "FREE SLAVES");
                combat(_XL_RAND() % 3 + 3, 40);
            } else if (mt == 3) {
                _XL_PRINT(10, 3, "DELIVER MESSAGE");
                combat(_XL_RAND() % 2 + 2, 25);
            } else {
                _XL_PRINT(10, 3, "GUARD VILLAGE");
                combat(_XL_RAND() % 3 + 1, 30);
            }
            if (rf <= 0) return;
            loot = (_XL_RAND() % 300) + 50;
            dn = dn + loot;
            wait_key();
        }
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(4, 4, "SERVICE COMPLETE!");
        wait_key();
        return;
    }
}

uint8_t check_hero(void) {
    if (ee == 6 && at > 200) return 1;
    return 0;
}

void hero_screen(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(4, 2, "YOU HAVE BECOME");
    _XL_PRINT(4, 4, "A DIVINE HERO!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 7, "CALLED TO THE FIRMAMENT");
    _XL_PRINT(2, 9, "WHERE THE QUASI-DEVS LIVE");
    _XL_PRINT(2, 12, "WEEKS SPENT:");
    _XL_PRINTD(14, 12, 3, tk);
    wait_key();
}

void game_over_screen(void) {
    uint16_t i;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(8, 6, "YOU DIED!");
    _XL_PING_SOUND();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(4, 10, "THE ELVES AND GNOMES");
    _XL_PRINT(4, 12, "SALUTE YOU AND WAIT FOR");
    _XL_PRINT(4, 14, "ANOTHER ADVENTURE");
    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(14, 17, "A D V E N T U R E");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(10, 20, "SYSTEMS EDITORIALE 1984");
    for (i = 0; i < 100; i++) {
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        uint8_t running;
        title_screen();
        char_select();
        running = 1;
        while (running) {
            clamp_stats();
            if (check_hero()) {
                hero_screen();
                running = 0;
                break;
            }
            if (rf <= 0) {
                game_over_screen();
                running = 0;
                break;
            }
            if (kf > 0 && rf < ro) {
                uint8_t c;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINT(2, 2, "USE HEALING FILTER? (1/2)");
                c = get_choice(2);
                if (c == 1) {
                    kf = kf - 1;
                    rf = ro;
                }
            }
            tavern();
        }
    }
    return 0;
}