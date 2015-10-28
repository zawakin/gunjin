
    var KOMAINF = {
        OUTOFBOARD: 64,
        EMPTY: 0,
        TAISHOU: 1,
        CHUJOU: 2,
        SHOUSHOU: 3,
        TAISA: 4,
        CHUUSA: 5,
        SHOUSA: 6,
        TAII: 7,
        CHUUI: 8,
        SHOUI: 9,
        KIHEI: 10,
        KOUHEI: 11,
        JIRAI: 12,
        HIKOUKI: 13,
        TANK: 14,
        SPY: 15,
        GUNKI: 16,
        ENEMY:32,
        ETAISHOU: 33,
        ECHUJOU: 34,
        ESHOUSHOU : 35,
        ETAISA: 36,
        ECHUUSA: 37,
        ESHOUSA: 38,
        ETAII: 39,
        CHUUI: 40,
        ESHOUI: 41,
        EKIHEI: 42,
        EKOUHEI: 43,
        EJIRAI: 44,
        EHIKOUKI: 45,
        ETANK: 46,
        ESPY: 47,
        EGUNKI: 48,
        E_HIDE:49
    }

    var SENGO = {
        SENTE: 1,
        GOTE:2
    }
    function isEnemy(komaInf) {
        return KOMAINF.ENEMY < komaInf && komaInf < KOMAINF.E_HIDE;
    }
    function isSelf(komaInf) {
        return KOMAINF.EMPTY < komaInf && komaInf < KOMAINF.ENEMY;
    }

    var komaStr = ["　　  ", "大将", "中将", "少将", "大佐", "中佐", "少佐", "大尉", "中尉", "少尉", "騎兵", "工兵", "地雷", "飛行", "タン", "スパ", "軍旗"];
    for (var i = 1; i <= KOMAINF.ENEMY; i++) {
        komaStr[i + KOMAINF.ENEMY] = komaStr[i];
    }
    //for (var i = 1; i <= 16; i++) {
    //    komaStr[i] = "  " + komaStr[i];
    //}
	var komaStrFULL = ["　　  ", "大将", "中将", "少将", "大佐", "中佐", "少佐", "大尉", "中尉", "少尉", "騎兵", "工兵", "地雷", "飛行機", "タンク", "スパイ", "軍旗"];
	for (var i = 1; i <= KOMAINF.ENEMY; i++) {
        komaStrFULL[i + KOMAINF.ENEMY] = komaStrFULL[i];
    }


    var Kyokumen = (function () {


        //コンストラクタ
        var Kyokumen = function () {
            this.suji = 6;
            this.dan = 8;
            this.deadKomas = [[], []];
            this.temaePlayer = 1;
            this.rule = GetIniRule();
            this.gameFinished = false;
            this.tesuu = 1;
            this.lastTe = null;
            console.log("Kyokumen Created!");

            this.board = [
                        [64, 64, 64, 64, 64, 64, 64, 64],
                        [64, 12, 12, 0, 12, 12, 12, 64],
                        [64, 0, 0, 12, 45, 1, 0, 64],
                        [64, 8, 9, 14, 14, 14, 11, 64],
                        [64, 3, 1, 39, 1, 6, 10, 64],
                        [64, 36, 36, 44, 45, 46, 40, 64],
                        [64, 12, 12, 12, 12, 12, 12, 64],
                        [64, 8, 9, 14, 14, 14, 11, 64],
                        [64, 3, 1, 1, 0, 6, 10, 64],
                        [64, 64, 64, 64, 64, 64, 64, 64]
            ];
            this.initData = {};
        }

        var p = Kyokumen.prototype;

        p.Start = function () {
            this.teban = SENGO.SENTE;
            return {
                board: this.board,
                deadKomas: this.deadKomas
            };
        };

        p.Next = function (Te) {
            var msg = "";
            switch (this.Battle(Te)) {
                case "勝ち":

                    break;
                case "負け":

                    break;
            }

            //終局しているかチェックし、しているならばどちらが勝ったかを返す
            if (this.FinishCheck()) {
                this.gameFinish = true;
            }
            this.teban = 3 - this.teban;
            this.tesuu++;
            return {board:this.board,
                deadKomas:this.deadKomas,
                resultMsg:msg,
                finish:false};
        };

        p.GetMikata = function () {
            if (this.teban == SENGO.SENTE) {
                return this.board.map(function (dan) {
                    return dan.map(function (x) {
                        if (x == KOMAINF.OUTOFBOARD) { return 64; }
                        if (((x & KOMAINF.ENEMY) >> 5) == 0 && (x != KOMAINF.EMPTY)) {
                            return 1;
                        } else {
                            return 0;
                        }
                    })
                });
            } else {
                return this.board.map(function (dan) {
                    return dan.map(function (x) {
                        if (x == KOMAINF.OUTOFBOARD) { return 64; }
                        return (x & KOMAINF.ENEMY) >> 5;
                    })
                })
            }
        };

        p.Enemy = function () {
            if (this.teban == 1) {
                return KOMAINF.ENEMY;
            } else {
                return 0;
            }
            return 0;
        };
        p.GetEnemys = function () {
            if (this.teban == SENGO.SENTE) {
                return this.board.map(function (dan) {
                    return dan.map(function (x) {
                        if (x == KOMAINF.OUTOFBOARD) { return 64; }
                        return (x & KOMAINF.ENEMY) >> 5;
                    })
                })
            } else {
                return this.board.map(function (dan) {
                    return dan.map(function (x) {
                        if (x == KOMAINF.OUTOFBOARD) { return 64; }
                        return ((x & KOMAINF.ENEMY) >> 5) == 0 && x == KOMAINF.EMPTY;
                    })
                })
            }
        };
        p.WhichPlayersKoma = function (pos) {
            var komaInf = this.board[pos.dan][pos.suji];
            if (((komaInf & KOMAINF.ENEMY) >> 5) == 1) {
                return 2;
            } else if (komaInf == KOMAINF.EMPTY) {
                return 0;
            }
            return 1;
        };
        p.Hanten = function () {
            var temp = [];
            this.board = this.board.reverse();
            for (var i = 1; i <= this.dan; i++) {
                for (var j = 1; j <= this.suji; j++) {
                    if (isSelf(this.board[i][j])) {
                        this.board[i][j] += KOMAINF.ENEMY;
                    } else {
                        if (isEnemy(this.board[i][j])) {
                            this.board[i][j] -= KOMAINF.ENEMY;
                        }
                    }
                }
                this.board[i] = this.board[i].reverse();
            }
            this.temaePlayer = 3 - this.temaePlayer;
            this.deadKomas = this.deadKomas.reverse();
            return this.board;
        };
        p.Print = function () {
            var ss = "                Player " + (3 - this.temaePlayer);
            if (this.teban != this.temaePlayer) {
                ss += "  TURN";
            }
            ss += "\n";
            if (this.temaePlayer == 1) {
                ss += "---+--6--+--5--+--4--+--3--+--2--+--1--+\n";
            } else {
                ss += "---+--1--+--2--+--3--+--4--+--5--+--6--+\n";
            }
            ss += "---+-----+-----+-----+-----+-----+-----+\n";
            for (var i = 1; i <= this.dan; i++) {
                if (this.temaePlayer == 1) {
                    ss += "-" + i + "-+-";
                } else {
                    ss += "-" + (this.dan + 1 - i) + "-+-";
                }
                for (var j = this.suji; j >= 1; j--) {
                    ss += "-" + komaStr[this.board[i][j]] + "-";
                }
                ss += "\n";
                if (i == 4) {
                    ss += "\n";
                }
            }

            ss += "                Player " + this.temaePlayer;
            if (this.teban == this.temaePlayer) {
                ss += "  TURN";
            }
            ss += "\n";
            for (var i = 1; i <= 2; i++) {
                ss += "player " + i + " 's Dead Koma List = ";
                for (var j = 0; j < this.deadKomas[i - 1].length; j++) {
                    ss += komaStr[this.deadKomas[i - 1][j] - 32 * (i - 1)] + ", ";
                }
                ss += "\n";
            }
            console.log(ss);
        };
        p.Move = function (teban, te) {
            var attackedKoma = this.board[te.To.dan][te.To.suji];
            console.log("attackedKoma = " + komaStr[attackedKoma]);
            if (te.TO)
                if ((attackedKoma % KOMAINF.ENEMY) < (te.komaInf % KOMAINF.ENEMY)) {
                    console.log(komaStr[te.komaInf] + " beats " + komaStr[attackedKoma]);
                }
        };
        p.InitialValidCheck = function () {
            return;
        }


        //盤面の表現 C -> javascript
        p.BoardExpFromC = function (board) {
            var movdom = board;
            var result = [];
            result[0] = [];
            result[this.dan + 1] = [];
            for (var i = 0; i <= this.suji + 1; i++) {
                result[0][i] = KOMAINF.OUTOFBOARD;
                result[this.dan + 1][i] = KOMAINF.OUTOFBOARD;
            }

            for (var dan = 1; dan <= this.dan; dan++) {
                result[dan] = [];
                result[dan][0] = KOMAINF.OUTOFBOARD;
                result[dan][this.suji + 1] = KOMAINF.OUTOFBOARD;
                for (var suji = 1; suji <= this.suji; suji++) {
                    result[dan][suji] = movdom[suji - 1][dan - 1];
                    if (17 <= result[dan][suji] && result[dan][suji] <= 32) {
                        result[dan][suji] += 16;
                    }
                }
            }
            return result;
        }

        //盤面の表現 javascript -> C
        p.BoardExpToC = function (board) {
            var b = [];
            for (var i = 0; i < this.suji; i++) {
                b[i] = [];
                for (var j = 0; j < this.dan; j++) {
                    b[i][j] = board[j + 1][i + 1];
                    if (isEnemy(b[i][j])) {
                        b[i][j] -= 16;
                    }
                }
            }
            return b;
        }

        p.GetMovableDomain = function (pos) {
            var pro = [pos.suji - 1, pos.dan - 1];
            var movdom = [];
            for (var i = 0; i < this.suji; i++) {
                movdom[i] = [];
            }

            var b = [];
            for (var i = 0; i < this.suji; i++) {
                b[i] = [];
                for (var j = 0; j < this.dan; j++) {
                    b[i][j] = this.board[j + 1][i + 1];
                }
            }

            console.log("b:");
            console.log(b);

            movableDomain(pro, b, movdom, this.tesuu, this.rule);

            console.log("movdom:");
            console.log(movdom);
            var result = [];
            result[0] = [];
            result[this.dan + 1] = [];
            for (var i = 0; i <= this.suji + 1; i++) {
                result[0][i] = KOMAINF.OUTOFBOARD;
                result[this.dan + 1][i] = KOMAINF.OUTOFBOARD;
            }

            for (var dan = 1; dan <= this.dan; dan++) {
                result[dan] = [];
                result[dan][0] = KOMAINF.OUTOFBOARD;
                result[dan][this.suji + 1] = KOMAINF.OUTOFBOARD;
                for (var suji = 1; suji <= this.suji; suji++) {
                    result[dan][suji] = movdom[suji - 1][dan - 1];
                }
            }
            console.log("result:");
            console.log(result);
            return result;
        }

        p.CreateInitBoardFromPlayers = function (senteBoard, goteBoard) {

            for (var dan = 5; dan <= this.dan; dan++) {
                for (var suji = 1; suji <= this.suji; suji++) {
                    this.board[dan][suji] = goteBoard[dan][suji];
                }
            }

            this.Hanten();
            for (var dan = 5; dan <= this.dan; dan++) {
                for (var suji = 1; suji <= this.suji; suji++) {
                    this.board[dan][suji] = senteBoard[dan][suji];
                }
            }

        };
        //flag:trueなら全て表向き
        p.GetSenteBoard = function (flag) {
            var resultBoard = [
                [17, 1, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 0, 0, 0, 0, 0, 0, 64],
                [64, 64, 64, 64, 64, 64, 64, 64]
            ];
            for (var dan = 1; dan <= this.dan; dan++) {
                for (var suji = 1; suji <= this.suji; suji++) {
                    if (isEnemy(this.board[dan][suji])) {
                        if (!flag) {
                            resultBoard[dan][suji] = KOMAINF.E_HIDE;
                        } else {
                            resultBoard[dan][suji] = this.board[dan][suji];
                        }
                    } else {
                        if (isSelf(this.board[dan][suji])) {
                            resultBoard[dan][suji] = this.board[dan][suji];
                        }
                    }

                }
            }
            return resultBoard;
        };

        p.GetGoteBoard = function (flag) {
            this.Hanten();
            var resultBoard = this.GetSenteBoard(flag);
            this.Hanten();
            return resultBoard;
        };

        p.GetGoteDeadKomas = function(){
        	var result = [[],[]];

    		for(var j=0;j<this.deadKomas[1].length;j++){
    			result[0][j] = this.deadKomas[1][j];
    		}
    		for(var j=0;j<this.deadKomas[0].length;j++){
    			result[1][j] = this.deadKomas[0][j];
    		}

        	return result;
        }

        p.TeToGote = function (te) {
            te.From.dan = this.dan + 1 - te.From.dan;
            te.From.suji = this.suji + 1 - te.From.suji;
            te.To.dan = this.dan + 1 - te.To.dan;
            te.To.suji = this.suji + 1 - te.To.suji;
            te.komaInf = KOMAINF.ENEMY + te.komaInf;
            return te;
        }

        //戦闘処理、手番が進む処理はここに書く
        p.Fight = function (te) {
            this.tesuu++;
            var sakiKomaInf = this.board[te.To.dan][te.To.suji];
            this.lastTe = new Te(
            	{dan:te.From.dan,suji:te.From.suji},
            	{dan:te.To.dan,suji:te.To.suji},
            	te.komaInf);


            this.lastTe.komaInf = 0;
            var pro = [te.From.suji-1,te.From.dan-1];
            var post = [te.To.suji - 1, te.To.dan - 1];

            var b = this.BoardExpToC(this.board);
            fight(pro, post, b, this.rule);
            this.board = this.BoardExpFromC(b);

            if(sakiKomaInf == KOMAINF.EMPTY)return;


            var beaten;
         	switch(this.board[te.To.dan][te.To.suji]){
        		case te.komaInf:
        			beaten = sakiKomaInf;
        			break;
        		case sakiKomaInf:
        			beaten = te.komaInf;
        			break;
        		case KOMAINF.EMPTY:
        			beaten = -1;
        			break;
        	}

        	if(beaten==-1){
        		if(isSelf(te.komaInf)){
        			this.deadKomas[0].push(te.komaInf);
        			this.deadKomas[1].push(sakiKomaInf);
        		}else{
        			this.deadKomas[1].push(te.komaInf);
        			this.deadKomas[0].push(sakiKomaInf);
        		}
        	}

        	if(isSelf(beaten)){
            	this.deadKomas[0].push(beaten);
        	}
        	if(isEnemy(beaten)){
            	this.deadKomas[1].push(beaten);
        	}

        };

        //符号の表現
        p.TeHugou = function (te) {
            var suuji = ["","１","２","３","４","５","６"];
            var kanji = ["","一", "二", "三", "四", "五", "六", "七", "八"];
            var result = this.tesuu + ":" +suuji[te.To.suji] + kanji[te.To.dan]  + komaStr[te.komaInf] + "(" + te.From.suji + te.From.dan + ")";
            return result;
        }

        //局面が詰んでいれば勝者
        p.FinishCheck = function () {
            var b = this.BoardExpToC(this.board);
            var num = victory(b, this.rule);
            return num;
        };

        p.DeadKomasToString = function(){
        	var result = [];
        	for(var i=0;i<=1;i++){
        		result[i] = [];
        		for(var j=0;j<this.deadKomas[i].length;j++){
        			result[i][j] = komaStrFULL[this.deadKomas[i][j]];
        		}
        	}
        	return result;
        }

        p.GetStrengthList = function(komainf){
        	var strengthList = [];
        	strengthList[0] = komaStrFULL[komainf];
        	strengthList[0] += "の強さ比較";
        	for(var i=1;i<=KOMAINF.GUNKI;i++){
        		var s = this.rule.unit[komainf].strength[i];
        		switch(s){
        			case 1:
        				s = "○";
        				break;
        			case 0:
        				s = "△";
        				break;
        			case -1:
        				s = "×";
        				break;
        		}
        		strengthList[i] = komaStrFULL[i] + " : " + s;
        	}
        	return strengthList;
        };

        return Kyokumen;
    })();


    function Pos(suji, dan) {
        this.suji = suji;
        this.dan = dan;
    }
    function Te(from,to,komaInf) {
        this.From = from;
        this.To = to;
        this.komaInf = komaInf;
        this.Print = function () {
            console.log((this.To).suji + "" + (this.To).dan + "" + komaStr[this.komaInf] + "(" + (this.From).suji + "" + this.From.dan + ")");
        }
    }

    function GetIniRule() {
        var inirule = {};
        inirule.classnum = 16;
        inirule.height = 8;
        inirule.width = 6;

        inirule.unit = [];
        for (var i = 0; i <= inirule.classnum; i++) {
            inirule.unit[i] = {};
            for (var j = 0; j < 4; j++) {
                inirule.unit[i].direction = [];
            }
            for (var j = 0; j < inirule.classnum; j++) {
                inirule.unit[i].strength = [];
            }
        }

        /* Null */

        inirule.unit[0].direction[0] = 0;
        inirule.unit[0].direction[1] = 0;
        inirule.unit[0].direction[2] = 0;
        inirule.unit[0].direction[3] = 0;

        inirule.unit[0].strength[0] = 0;
        inirule.unit[0].strength[1] = -1;
        inirule.unit[0].strength[2] = -1;
        inirule.unit[0].strength[3] = -1;
        inirule.unit[0].strength[4] = -1;
        inirule.unit[0].strength[5] = -1;
        inirule.unit[0].strength[6] = -1;
        inirule.unit[0].strength[7] = -1;
        inirule.unit[0].strength[8] = -1;
        inirule.unit[0].strength[9] = -1;
        inirule.unit[0].strength[10] = -1;
        inirule.unit[0].strength[11] = -1;
        inirule.unit[0].strength[12] = -1;
        inirule.unit[0].strength[13] = -1;
        inirule.unit[0].strength[14] = -1;
        inirule.unit[0].strength[15] = -1;
        inirule.unit[0].strength[16] = -1;

        inirule.unit[0].number = 0;

        /* 大将 */

        inirule.unit[1].direction[0] = 1;
        inirule.unit[1].direction[1] = 1;
        inirule.unit[1].direction[2] = 1;
        inirule.unit[1].direction[3] = 1;

        inirule.unit[1].strength[0] = 1;
        inirule.unit[1].strength[1] = 0;
        inirule.unit[1].strength[2] = 1;
        inirule.unit[1].strength[3] = 1;
        inirule.unit[1].strength[4] = 1;
        inirule.unit[1].strength[5] = 1;
        inirule.unit[1].strength[6] = 1;
        inirule.unit[1].strength[7] = 1;
        inirule.unit[1].strength[8] = 1;
        inirule.unit[1].strength[9] = 1;
        inirule.unit[1].strength[10] = 1;
        inirule.unit[1].strength[11] = 1;
        inirule.unit[1].strength[12] = 0;
        inirule.unit[1].strength[13] = 1;
        inirule.unit[1].strength[14] = 1;
        inirule.unit[1].strength[15] = -1;
        inirule.unit[1].strength[16] = 1;

        inirule.unit[1].number = 1;

        /* 中将 */

        inirule.unit[2].direction[0] = 1;
        inirule.unit[2].direction[1] = 1;
        inirule.unit[2].direction[2] = 1;
        inirule.unit[2].direction[3] = 1;

        inirule.unit[2].strength[0] = 1;
        inirule.unit[2].strength[1] = -1;
        inirule.unit[2].strength[2] = 0;
        inirule.unit[2].strength[3] = 1;
        inirule.unit[2].strength[4] = 1;
        inirule.unit[2].strength[5] = 1;
        inirule.unit[2].strength[6] = 1;
        inirule.unit[2].strength[7] = 1;
        inirule.unit[2].strength[8] = 1;
        inirule.unit[2].strength[9] = 1;
        inirule.unit[2].strength[10] = 1;
        inirule.unit[2].strength[11] = 1;
        inirule.unit[2].strength[12] = 0;
        inirule.unit[2].strength[13] = 1;
        inirule.unit[2].strength[14] = 1;
        inirule.unit[2].strength[15] = 1;
        inirule.unit[2].strength[16] = 1;

        inirule.unit[2].number = 1;

        /* 少将 */

        inirule.unit[3].direction[0] = 1;
        inirule.unit[3].direction[1] = 1;
        inirule.unit[3].direction[2] = 1;
        inirule.unit[3].direction[3] = 1;

        inirule.unit[3].strength[0] = 1;
        inirule.unit[3].strength[1] = -1;
        inirule.unit[3].strength[2] = -1;
        inirule.unit[3].strength[3] = 0;
        inirule.unit[3].strength[4] = 1;
        inirule.unit[3].strength[5] = 1;
        inirule.unit[3].strength[6] = 1;
        inirule.unit[3].strength[7] = 1;
        inirule.unit[3].strength[8] = 1;
        inirule.unit[3].strength[9] = 1;
        inirule.unit[3].strength[10] = 1;
        inirule.unit[3].strength[11] = 1;
        inirule.unit[3].strength[12] = 0;
        inirule.unit[3].strength[13] = 1;
        inirule.unit[3].strength[14] = 1;
        inirule.unit[3].strength[15] = 1;
        inirule.unit[3].strength[16] = 1;

        inirule.unit[3].number = 1;

        /* 大佐 */

        inirule.unit[4].direction[0] = 1;
        inirule.unit[4].direction[1] = 1;
        inirule.unit[4].direction[2] = 1;
        inirule.unit[4].direction[3] = 1;

        inirule.unit[4].strength[0] = 1;
        inirule.unit[4].strength[1] = -1;
        inirule.unit[4].strength[2] = -1;
        inirule.unit[4].strength[3] = -1;
        inirule.unit[4].strength[4] = 0;
        inirule.unit[4].strength[5] = 1;
        inirule.unit[4].strength[6] = 1;
        inirule.unit[4].strength[7] = 1;
        inirule.unit[4].strength[8] = 1;
        inirule.unit[4].strength[9] = 1;
        inirule.unit[4].strength[10] = 1;
        inirule.unit[4].strength[11] = 1;
        inirule.unit[4].strength[12] = 0;
        inirule.unit[4].strength[13] = -1;
        inirule.unit[4].strength[14] = -1;
        inirule.unit[4].strength[15] = 1;
        inirule.unit[4].strength[16] = 1;

        inirule.unit[4].number = 1;

        /* 中佐 */

        inirule.unit[5].direction[0] = 1;
        inirule.unit[5].direction[1] = 1;
        inirule.unit[5].direction[2] = 1;
        inirule.unit[5].direction[3] = 1;

        inirule.unit[5].strength[0] = 1;
        inirule.unit[5].strength[1] = -1;
        inirule.unit[5].strength[2] = -1;
        inirule.unit[5].strength[3] = -1;
        inirule.unit[5].strength[4] = -1;
        inirule.unit[5].strength[5] = 0;
        inirule.unit[5].strength[6] = 1;
        inirule.unit[5].strength[7] = 1;
        inirule.unit[5].strength[8] = 1;
        inirule.unit[5].strength[9] = 1;
        inirule.unit[5].strength[10] = 1;
        inirule.unit[5].strength[11] = 1;
        inirule.unit[5].strength[12] = 0;
        inirule.unit[5].strength[13] = -1;
        inirule.unit[5].strength[14] = -1;
        inirule.unit[5].strength[15] = 1;
        inirule.unit[5].strength[16] = 1;

        inirule.unit[5].number = 1;

        /* 少佐 */

        inirule.unit[6].direction[0] = 1;
        inirule.unit[6].direction[1] = 1;
        inirule.unit[6].direction[2] = 1;
        inirule.unit[6].direction[3] = 1;

        inirule.unit[6].strength[0] = 1;
        inirule.unit[6].strength[1] = -1;
        inirule.unit[6].strength[2] = -1;
        inirule.unit[6].strength[3] = -1;
        inirule.unit[6].strength[4] = -1;
        inirule.unit[6].strength[5] = -1;
        inirule.unit[6].strength[6] = 0;
        inirule.unit[6].strength[7] = 1;
        inirule.unit[6].strength[8] = 1;
        inirule.unit[6].strength[9] = 1;
        inirule.unit[6].strength[10] = 1;
        inirule.unit[6].strength[11] = 1;
        inirule.unit[6].strength[12] = 0;
        inirule.unit[6].strength[13] = -1;
        inirule.unit[6].strength[14] = -1;
        inirule.unit[6].strength[15] = 1;
        inirule.unit[6].strength[16] = 1;

        inirule.unit[6].number = 1;

        /* 大尉 */

        inirule.unit[7].direction[0] = 1;
        inirule.unit[7].direction[1] = 1;
        inirule.unit[7].direction[2] = 1;
        inirule.unit[7].direction[3] = 1;

        inirule.unit[7].strength[0] = 1;
        inirule.unit[7].strength[1] = -1;
        inirule.unit[7].strength[2] = -1;
        inirule.unit[7].strength[3] = -1;
        inirule.unit[7].strength[4] = -1;
        inirule.unit[7].strength[5] = -1;
        inirule.unit[7].strength[6] = -1;
        inirule.unit[7].strength[7] = 0;
        inirule.unit[7].strength[8] = 1;
        inirule.unit[7].strength[9] = 1;
        inirule.unit[7].strength[10] = 1;
        inirule.unit[7].strength[11] = 1;
        inirule.unit[7].strength[12] = 0;
        inirule.unit[7].strength[13] = -1;
        inirule.unit[7].strength[14] = -1;
        inirule.unit[7].strength[15] = 1;
        inirule.unit[7].strength[16] = 1;

        inirule.unit[7].number = 2;

        /* 中尉 */

        inirule.unit[8].direction[0] = 1;
        inirule.unit[8].direction[1] = 1;
        inirule.unit[8].direction[2] = 1;
        inirule.unit[8].direction[3] = 1;

        inirule.unit[8].strength[0] = 1;
        inirule.unit[8].strength[1] = -1;
        inirule.unit[8].strength[2] = -1;
        inirule.unit[8].strength[3] = -1;
        inirule.unit[8].strength[4] = -1;
        inirule.unit[8].strength[5] = -1;
        inirule.unit[8].strength[6] = -1;
        inirule.unit[8].strength[7] = -1;
        inirule.unit[8].strength[8] = 0;
        inirule.unit[8].strength[9] = 1;
        inirule.unit[8].strength[10] = 1;
        inirule.unit[8].strength[11] = 1;
        inirule.unit[8].strength[12] = 0;
        inirule.unit[8].strength[13] = -1;
        inirule.unit[8].strength[14] = -1;
        inirule.unit[8].strength[15] = 1;
        inirule.unit[8].strength[16] = 1;

        inirule.unit[8].number = 2;

        /* 少尉 */

        inirule.unit[9].direction[0] = 1;
        inirule.unit[9].direction[1] = 1;
        inirule.unit[9].direction[2] = 1;
        inirule.unit[9].direction[3] = 1;

        inirule.unit[9].strength[0] = 1;
        inirule.unit[9].strength[1] = -1;
        inirule.unit[9].strength[2] = -1;
        inirule.unit[9].strength[3] = -1;
        inirule.unit[9].strength[4] = -1;
        inirule.unit[9].strength[5] = -1;
        inirule.unit[9].strength[6] = -1;
        inirule.unit[9].strength[7] = -1;
        inirule.unit[9].strength[8] = -1;
        inirule.unit[9].strength[9] = 0;
        inirule.unit[9].strength[10] = 1;
        inirule.unit[9].strength[11] = 1;
        inirule.unit[9].strength[12] = 0;
        inirule.unit[9].strength[13] = -1;
        inirule.unit[9].strength[14] = -1;
        inirule.unit[9].strength[15] = 1;
        inirule.unit[9].strength[16] = 1;

        inirule.unit[9].number = 2;

        /* 騎兵 */

        inirule.unit[10].direction[0] = 1;
        inirule.unit[10].direction[1] = 2;
        inirule.unit[10].direction[2] = 1;
        inirule.unit[10].direction[3] = 1;

        inirule.unit[10].strength[0] = 1;
        inirule.unit[10].strength[1] = -1;
        inirule.unit[10].strength[2] = -1;
        inirule.unit[10].strength[3] = -1;
        inirule.unit[10].strength[4] = -1;
        inirule.unit[10].strength[5] = -1;
        inirule.unit[10].strength[6] = -1;
        inirule.unit[10].strength[7] = -1;
        inirule.unit[10].strength[8] = -1;
        inirule.unit[10].strength[9] = -1;
        inirule.unit[10].strength[10] = 0;
        inirule.unit[10].strength[11] = 1;
        inirule.unit[10].strength[12] = 0;
        inirule.unit[10].strength[13] = -1;
        inirule.unit[10].strength[14] = -1;
        inirule.unit[10].strength[15] = 1;
        inirule.unit[10].strength[16] = 1;

        inirule.unit[10].number = 1;

        /* 工兵 */

        inirule.unit[11].direction[0] = 15;
        inirule.unit[11].direction[1] = 15;
        inirule.unit[11].direction[2] = 15;
        inirule.unit[11].direction[3] = 15;

        inirule.unit[11].strength[0] = 1;
        inirule.unit[11].strength[1] = -1;
        inirule.unit[11].strength[2] = -1;
        inirule.unit[11].strength[3] = -1;
        inirule.unit[11].strength[4] = -1;
        inirule.unit[11].strength[5] = -1;
        inirule.unit[11].strength[6] = -1;
        inirule.unit[11].strength[7] = -1;
        inirule.unit[11].strength[8] = -1;
        inirule.unit[11].strength[9] = -1;
        inirule.unit[11].strength[10] = -1;
        inirule.unit[11].strength[11] = 0;
        inirule.unit[11].strength[12] = 1;
        inirule.unit[11].strength[13] = -1;
        inirule.unit[11].strength[14] = 1;
        inirule.unit[11].strength[15] = 1;
        inirule.unit[11].strength[16] = 1;

        inirule.unit[11].number = 2;

        /* 地雷 */

        inirule.unit[12].direction[0] = 0;
        inirule.unit[12].direction[1] = 0;
        inirule.unit[12].direction[2] = 0;
        inirule.unit[12].direction[3] = 0;

        inirule.unit[12].strength[0] = 1;
        inirule.unit[12].strength[1] = 0;
        inirule.unit[12].strength[2] = 0;
        inirule.unit[12].strength[3] = 0;
        inirule.unit[12].strength[4] = 0;
        inirule.unit[12].strength[5] = 0;
        inirule.unit[12].strength[6] = 0;
        inirule.unit[12].strength[7] = 0;
        inirule.unit[12].strength[8] = 0;
        inirule.unit[12].strength[9] = 0;
        inirule.unit[12].strength[10] = 0;
        inirule.unit[12].strength[11] = -1;
        inirule.unit[12].strength[12] = 0;
        inirule.unit[12].strength[13] = -1;
        inirule.unit[12].strength[14] = 0;
        inirule.unit[12].strength[15] = 0;
        inirule.unit[12].strength[16] = 1;

        inirule.unit[12].number = 2;

        /* 飛行機 */

        inirule.unit[13].direction[0] = 1;
        inirule.unit[13].direction[1] = 15;
        inirule.unit[13].direction[2] = 1;
        inirule.unit[13].direction[3] = 15;

        inirule.unit[13].strength[0] = 1;
        inirule.unit[13].strength[1] = -1;
        inirule.unit[13].strength[2] = -1;
        inirule.unit[13].strength[3] = -1;
        inirule.unit[13].strength[4] = 1;
        inirule.unit[13].strength[5] = 1;
        inirule.unit[13].strength[6] = 1;
        inirule.unit[13].strength[7] = 1;
        inirule.unit[13].strength[8] = 1;
        inirule.unit[13].strength[9] = 1;
        inirule.unit[13].strength[10] = 1;
        inirule.unit[13].strength[11] = 1;
        inirule.unit[13].strength[12] = 1;
        inirule.unit[13].strength[13] = 0;
        inirule.unit[13].strength[14] = 1;
        inirule.unit[13].strength[15] = 1;
        inirule.unit[13].strength[16] = 1;

        inirule.unit[13].number = 2;

        /* タンク */

        inirule.unit[14].direction[0] = 1;
        inirule.unit[14].direction[1] = 2;
        inirule.unit[14].direction[2] = 1;
        inirule.unit[14].direction[3] = 1;

        inirule.unit[14].strength[0] = 1;
        inirule.unit[14].strength[1] = -1;
        inirule.unit[14].strength[2] = -1;
        inirule.unit[14].strength[3] = -1;
        inirule.unit[14].strength[4] = 1;
        inirule.unit[14].strength[5] = 1;
        inirule.unit[14].strength[6] = 1;
        inirule.unit[14].strength[7] = 1;
        inirule.unit[14].strength[8] = 1;
        inirule.unit[14].strength[9] = 1;
        inirule.unit[14].strength[10] = 1;
        inirule.unit[14].strength[11] = -1;
        inirule.unit[14].strength[12] = 0;
        inirule.unit[14].strength[13] = -1;
        inirule.unit[14].strength[14] = 0;
        inirule.unit[14].strength[15] = 1;
        inirule.unit[14].strength[16] = 1;

        inirule.unit[14].number = 2;

        /* スパイ */

        inirule.unit[15].direction[0] = 1;
        inirule.unit[15].direction[1] = 1;
        inirule.unit[15].direction[2] = 1;
        inirule.unit[15].direction[3] = 1;

        inirule.unit[15].strength[0] = 1;
        inirule.unit[15].strength[1] = 1;
        inirule.unit[15].strength[2] = -1;
        inirule.unit[15].strength[3] = -1;
        inirule.unit[15].strength[4] = -1;
        inirule.unit[15].strength[5] = -1;
        inirule.unit[15].strength[6] = -1;
        inirule.unit[15].strength[7] = -1;
        inirule.unit[15].strength[8] = -1;
        inirule.unit[15].strength[9] = -1;
        inirule.unit[15].strength[10] = -1;
        inirule.unit[15].strength[11] = -1;
        inirule.unit[15].strength[12] = -1;
        inirule.unit[15].strength[13] = -1;
        inirule.unit[15].strength[14] = -1;
        inirule.unit[15].strength[15] = 0;
        inirule.unit[15].strength[16] = 1;

        inirule.unit[15].number = 1;

        /* 軍旗 */

        inirule.unit[16].direction[0] = 0;
        inirule.unit[16].direction[1] = 0;
        inirule.unit[16].direction[2] = 0;
        inirule.unit[16].direction[3] = 0;

        inirule.unit[16].strength[0] = 1;
        inirule.unit[16].strength[1] = -1;
        inirule.unit[16].strength[2] = -1;
        inirule.unit[16].strength[3] = -1;
        inirule.unit[16].strength[4] = -1;
        inirule.unit[16].strength[5] = -1;
        inirule.unit[16].strength[6] = -1;
        inirule.unit[16].strength[7] = -1;
        inirule.unit[16].strength[8] = -1;
        inirule.unit[16].strength[9] = -1;
        inirule.unit[16].strength[10] = -1;
        inirule.unit[16].strength[11] = -1;
        inirule.unit[16].strength[12] = -1;
        inirule.unit[16].strength[13] = -1;
        inirule.unit[16].strength[14] = -1;
        inirule.unit[16].strength[15] = -1;
        inirule.unit[16].strength[16] = 0;

        inirule.unit[16].number = 1;
        return inirule;
    }

    function movableDomain(pro, board, domain, turn, rule) {
        var i, j;
        var k, l, m;
        var error = -1;

        for (i = 0 ; i < rule.width ; i++) {
            for (j = 0 ; j < rule.height ; j++) {
                domain[i][j] = 0;
            }
        }

        if ((pro[0] >= 0 && pro[0] < rule.width) && (pro[1] >= 0 && pro[1] < rule.height)) {
            if (pro[0] == rule.width / 2 && pro[1] == 0) {
                pro[0] = rule.width / 2 - 1;
            } else if (pro[0] == rule.width / 2 - 1 && pro[1] == rule.height - 1) {
                pro[0] = rule.width / 2;
            }
            if (turn % 2 == 1 && (board[pro[0]][pro[1]] > 0 && board[pro[0]][pro[1]] <= rule.classnum)) {
                error = 0;
                k = 1;
                l = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]]].direction[0] && pro[0] - l >= 0) {
                    if (board[pro[0]][pro[1]] != 13) {
                        if ((pro[0] - l == rule.width / 2 && pro[1] == 0) || (pro[0] - l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] - l][pro[1]] == 0) {
                                domain[pro[0] - l][pro[1]] = 1;
                            } else if (board[pro[0] - l][pro[1]] > rule.classnum) {
                                domain[pro[0] - l][pro[1]] = 1;
                                break;
                            } else {
                                break;
                            }
                            k++;
                            l++;
                        }
                    } else {
                        if ((pro[0] - l == rule.width / 2 && pro[1] == 0) || (pro[0] - l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] - l][pro[1]] == 0 || board[pro[0] - l][pro[1]] > rule.classnum) {
                                domain[pro[0] - l][pro[1]] = 1;
                            }
                            k++;
                            l++;
                        }
                    }
                }
                k = 1;
                l = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]]].direction[2] && pro[0] + l < rule.width) {
                    if (board[pro[0]][pro[1]] != 13) {
                        if ((pro[0] + l == rule.width / 2 && pro[1] == 0) || (pro[0] + l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] + l][pro[1]] == 0) {
                                domain[pro[0] + l][pro[1]] = 1;
                            } else if (board[pro[0] + l][pro[1]] > rule.classnum) {
                                domain[pro[0] + l][pro[1]] = 1;
                                break;
                            } else {
                                break;
                            }
                            k++;
                            l++;
                        }
                    } else {
                        if ((pro[0] + l == rule.width / 2 && pro[1] == 0) || (pro[0] + l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] + l][pro[1]] == 0 || board[pro[0] + l][pro[1]] > rule.classnum) {
                                domain[pro[0] + l][pro[1]] = 1;
                            }
                            k++;
                            l++;
                        }
                    }
                }
                k = 1;
                l = 1;
                m = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]]].direction[1] && pro[1] - k >= 0) {
                    if (board[pro[0]][pro[1]] != 13) {
                        if (m == 1 && (pro[0] == rule.width / 2 && pro[1] == rule.height - 1)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]]].direction[1] && pro[1] - l >= rule.height / 2) {
                                if (board[pro[0] - 1][pro[1] - l] == 0) {
                                    domain[pro[0] - 1][pro[1] - l] = 1;
                                } else if (board[pro[0] - 1][pro[1] - l] > rule.classnum) {
                                    domain[pro[0] - 1][pro[1] - l] = 1;
                                    break;
                                } else {
                                    break;
                                }
                                l++;
                            }
                            m++;
                        } else if (!((pro[0] != 1 && pro[0] != rule.width - 2) && pro[1] - k == rule.height / 2 - 1)) {
                            if (pro[0] == rule.width / 2 && pro[1] - k == 0) {
                                if (board[pro[0] - 1][pro[1] - k] == 0) {
                                    domain[pro[0] - 1][pro[1] - k] = 1;
                                } else if (board[pro[0] - 1][pro[1] - k] > rule.classnum) {
                                    domain[pro[0] - 1][pro[1] - k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            } else {
                                if (board[pro[0]][pro[1] - k] == 0) {
                                    domain[pro[0]][pro[1] - k] = 1;
                                } else if (board[pro[0]][pro[1] - k] > rule.classnum) {
                                    domain[pro[0]][pro[1] - k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            }
                            k++;
                        } else {
                            break;
                        }
                    } else {
                        if (m == 1 && (pro[0] == rule.width / 2 && pro[1] == rule.height - 1)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]]].direction[1] && pro[1] - l >= 0) {
                                if (board[pro[0] - 1][pro[1] - l] == 0 || board[pro[0] - 1][pro[1] - l] > rule.classnum) {
                                    domain[pro[0] - 1][pro[1] - l] = 1;
                                }
                                l++;
                            }
                            m++;
                        } else {
                            if ((pro[0] == rule.width / 2 && pro[1] - k == 0) && (board[pro[0] - 1][pro[1] - k] == 0 || board[pro[0] - 1][pro[1] - k] > rule.classnum)) {
                                domain[pro[0] - 1][pro[1] - k] = 1;
                            } else if (!(pro[0] == rule.width / 2 && pro[1] - k == 0) && (board[pro[0]][pro[1] - k] == 0 || board[pro[0]][pro[1] - k] > rule.classnum)) {
                                domain[pro[0]][pro[1] - k] = 1;
                            }
                            k++;
                        }
                    }
                }
                k = 1;
                l = 1;
                m = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]]].direction[3] && pro[1] + k < rule.height) {
                    if (board[pro[0]][pro[1]] != 13) {
                        if (m == 1 && (pro[0] == rule.width / 2 - 1 && pro[1] == 0)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]]].direction[3] && pro[1] + l <= rule.height / 2 - 1) {
                                if (board[pro[0] + 1][pro[1] + l] == 0) {
                                    domain[pro[0] + 1][pro[1] + l] = 1;
                                } else if (board[pro[0] + 1][pro[1] + l] > rule.classnum) {
                                    domain[pro[0] + 1][pro[1] + l] = 1;
                                    break;
                                } else {
                                    break;
                                }
                                l++;
                            }
                            m++;
                        } else if (!((pro[0] != 1 && pro[0] != rule.width - 2) && pro[1] + k == rule.height / 2)) {
                            if (pro[0] == rule.width / 2 - 1 && pro[1] + k == rule.height - 1) {
                                if (board[pro[0] + 1][pro[1] + k] == 0) {
                                    domain[pro[0] + 1][pro[1] + k] = 1;
                                } else if (board[pro[0] + 1][pro[1] + k] > rule.classnum) {
                                    domain[pro[0] + 1][pro[1] + k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            } else {
                                if (board[pro[0]][pro[1] + k] == 0) {
                                    domain[pro[0]][pro[1] + k] = 1;
                                } else if (board[pro[0]][pro[1] + k] > rule.classnum) {
                                    domain[pro[0]][pro[1] + k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            }
                            k++;
                        } else {
                            break;
                        }
                    } else {
                        if (m == 1 && (pro[0] == rule.width / 2 - 1 && pro[1] == 0)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]]].direction[3] && pro[1] + l < rule.height) {
                                if (board[pro[0] + 1][pro[1] + l] == 0 || board[pro[0] + 1][pro[1] + l] > rule.classnum) {
                                    domain[pro[0] + 1][pro[1] + l] = 1;
                                }
                                l++;
                            }
                            m++;
                        } else {
                            if ((pro[0] == rule.width / 2 - 1 && pro[1] + k == rule.height - 1) && (board[pro[0] + 1][pro[1] + k] == 0 || board[pro[0] + 1][pro[1] + k] > rule.classnum)) {
                                domain[pro[0] + 1][pro[1] + k] = 1;
                            } else if (!(pro[0] == rule.width / 2 - 1 && pro[1] + k == rule.height - 1) && (board[pro[0]][pro[1] + k] == 0 || board[pro[0]][pro[1] + k] > rule.classnum)) {
                                domain[pro[0]][pro[1] + k] = 1;
                            }
                            k++;
                        }
                    }
                }
            } else if (turn % 2 == 0 && board[pro[0]][pro[1]] > rule.classnum) {
                error = 0;
                k = 1;
                l = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[2] && pro[0] - l >= 0) {
                    if (board[pro[0]][pro[1]] - rule.classnum != 13) {
                        if ((pro[0] - l == rule.width / 2 && pro[1] == 0) || (pro[0] - l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] - l][pro[1]] == 0) {
                                domain[pro[0] - l][pro[1]] = 1;
                            } else if (board[pro[0] - l][pro[1]] > 0 && board[pro[0] - l][pro[1]] <= rule.classnum) {
                                domain[pro[0] - l][pro[1]] = 1;
                                break;
                            } else {
                                break;
                            }
                            k++;
                            l++;
                        }
                    } else {
                        if ((pro[0] - l == rule.width / 2 && pro[1] == 0) || (pro[0] - l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] - l][pro[1]] <= rule.classnum) {
                                domain[pro[0] - l][pro[1]] = 1;
                            }
                            k++;
                            l++;
                        }
                    }
                }
                k = 1;
                l = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[0] && pro[0] + l < rule.width) {
                    if (board[pro[0]][pro[1] - rule.classnum] != 13) {
                        if ((pro[0] + l == rule.width / 2 && pro[1] == 0) || (pro[0] + l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] + l][pro[1]] == 0) {
                                domain[pro[0] + l][pro[1]] = 1;
                            } else if (board[pro[0] + l][pro[1]] > 0 && board[pro[0] + l][pro[1]] <= rule.classnum) {
                                domain[pro[0] + l][pro[1]] = 1;
                                break;
                            } else {
                                break;
                            }
                            k++;
                            l++;
                        }
                    } else {
                        if ((pro[0] + l == rule.width / 2 && pro[1] == 0) || (pro[0] + l == rule.width / 2 - 1 && pro[1] == rule.height - 1)) {
                            l++;
                        } else {
                            if (board[pro[0] + l][pro[1]] <= rule.classnum) {
                                domain[pro[0] + l][pro[1]] = 1;
                            }
                            k++;
                            l++;
                        }
                    }
                }
                k = 1;
                l = 1;
                m = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[3] && pro[1] - k >= 0) {
                    if (board[pro[0]][pro[1]] - rule.classnum != 13) {
                        if (m == 1 && (pro[0] == rule.width / 2 && pro[1] == rule.height - 1)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[3] && pro[1] - l >= rule.height / 2) {
                                if (board[pro[0] - 1][pro[1] - l] == 0) {
                                    domain[pro[0] - 1][pro[1] - l] = 1;
                                } else if (board[pro[0] - 1][pro[1] - l] > 0 && board[pro[0] - 1][pro[1] - l] <= rule.classnum) {
                                    domain[pro[0] - 1][pro[1] - l] = 1;
                                    break;
                                } else {
                                    break;
                                }
                                l++;
                            }
                            m++;
                        } else if (!((pro[0] != 1 && pro[0] != rule.width - 2) && pro[1] - k == rule.height / 2 - 1)) {
                            if (pro[0] == rule.width / 2 && pro[1] - k == 0) {
                                if (board[pro[0] - 1][pro[1] - k] == 0) {
                                    domain[pro[0] - 1][pro[1] - k] = 1;
                                } else if (board[pro[0] - 1][pro[1] - k] > 0 && board[pro[0] - 1][pro[1] - k] <= rule.classnum) {
                                    domain[pro[0] - 1][pro[1] - k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            } else {
                                if (board[pro[0]][pro[1] - k] == 0) {
                                    domain[pro[0]][pro[1] - k] = 1;
                                } else if (board[pro[0]][pro[1] - k] > 0 && board[pro[0]][pro[1] - k] <= rule.classnum) {
                                    domain[pro[0]][pro[1] - k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            }
                            k++;
                        } else {
                            break;
                        }
                    } else {
                        if (m == 1 && (pro[0] == rule.width / 2 && pro[1] == rule.height - 1)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[3] && pro[1] - l >= 0) {
                                if (board[pro[0] - 1][pro[1] - l] <= rule.classnum) {
                                    domain[pro[0] - 1][pro[1] - l] = 1;
                                }
                                l++;
                            }
                            m++;
                        } else {
                            if ((pro[0] == rule.width / 2 && pro[1] - k == 0) && board[pro[0] - 1][pro[1] - k] <= rule.classnum) {
                                domain[pro[0] - 1][pro[1] - k] = 1;
                            } else if (!(pro[0] == rule.width / 2 && pro[1] - k == 0) && board[pro[0]][pro[1] - k] <= rule.classnum) {
                                domain[pro[0]][pro[1] - k] = 1;
                            }
                            k++;
                        }
                    }
                }
                k = 1;
                l = 1;
                m = 1;
                while (k <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[1] && pro[1] + k < rule.height) {
                    if (board[pro[0]][pro[1]] - rule.classnum != 13) {
                        if (m == 1 && (pro[0] == rule.width / 2 - 1 && pro[1] == 0)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[1] && pro[1] + l <= rule.height / 2 - 1) {
                                if (board[pro[0] + 1][pro[1] + l] == 0) {
                                    domain[pro[0] + 1][pro[1] + l] = 1;
                                } else if (board[pro[0] + 1][pro[1] + l] > 0 && board[pro[0] + 1][pro[1] + l] <= rule.classnum) {
                                    domain[pro[0] + 1][pro[1] + l] = 1;
                                    break;
                                } else {
                                    break;
                                }
                                l++;
                            }
                            m++;
                        } else if (!((pro[0] != 1 && pro[0] != rule.width - 2) && pro[1] + k == rule.height / 2)) {
                            if (pro[0] == rule.width / 2 - 1 && pro[1] + k == rule.height - 1) {
                                if (board[pro[0] + 1][pro[1] + k] == 0) {
                                    domain[pro[0] + 1][pro[1] + k] = 1;
                                } else if (board[pro[0] + 1][pro[1] + k] > 0 && board[pro[0] + 1][pro[1] + k] <= rule.classnum) {
                                    domain[pro[0] + 1][pro[1] + k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            } else {
                                if (board[pro[0]][pro[1] + k] == 0) {
                                    domain[pro[0]][pro[1] + k] = 1;
                                } else if (board[pro[0]][pro[1] + k] > 0 && board[pro[0]][pro[1] + k] <= rule.classnum) {
                                    domain[pro[0]][pro[1] + k] = 1;
                                    break;
                                } else {
                                    break;
                                }
                            }
                            k++;
                        } else {
                            break;
                        }
                    } else {
                        if (m == 1 && (pro[0] == rule.width / 2 - 1 && pro[1] == 0)) {
                            while (l <= rule.unit[board[pro[0]][pro[1]] - rule.classnum].direction[1] && pro[1] + l < rule.height) {
                                if (board[pro[0] + 1][pro[1] + l] <= rule.classnum) {
                                    domain[pro[0] + 1][pro[1] + l] = 1;
                                }
                                l++;
                            }
                            m++;
                        } else {
                            if ((pro[0] == rule.width / 2 - 1 && pro[1] + k == rule.height - 1) && board[pro[0] + 1][pro[1] + k] <= rule.classnum) {
                                domain[pro[0] + 1][pro[1] + k] = 1;
                            } else if (!(pro[0] == rule.width / 2 - 1 && pro[1] + k == rule.height - 1) && board[pro[0]][pro[1] + k] <= rule.classnum) {
                                domain[pro[0]][pro[1] + k] = 1;
                            }
                            k++;
                        }
                    }
                }
            }
        }
        return error;
    }

/* 勝利条件の判定 */ /* return = 1: 先攻の勝利, 2: 後攻の勝利, 3: 引き分け */
    function victory(board,rule)
    {
	var first, second ;
    var i, j ;
    var vic = 0 ;

    if (board[rule.width/2-1][0] >= 1 && board[rule.width/2-1][0] <= 6){
        vic = 1 ;
    } else if (board[rule.width/2][rule.height-1] >= 1+rule.classnum && board[rule.width/2][rule.height-1] <= 6+rule.classnum){
        vic = 2 ;
    }

    first = 0 ;
    second = 0 ;
    for (i = 0 ; i < rule.width ; i++){
        for (j = 0 ; j < rule.height ; j++){
            if ((board[i][j] > 0 && board[i][j] <= rule.classnum) && !(board[i][j] == 12 || board[i][j] == 16)){
                first++ ;
            } else if (board[i][j] > rule.classnum && !(board[i][j]-rule.classnum == 12 || board[i][j]-rule.classnum == 16)){
                second++ ;
            }
        }
    }
    if (first > 0 && second == 0){
        vic = 1 ;
    }else if (first == 0 && second > 0){
        vic = 2 ;
    }else if (first == 0 && second == 0){
        vic = 3 ;
    }
    return vic ;
    }

//pro mae
//post ato
//board battlego henka
//rul ok

    function fight(pro, post, board, rule) {
        if (board[post[0]][post[1]] > rule.classnum) {
            if (board[post[0]][post[1]] == 16 + rule.classnum && (post[0] == rule.width / 2 - 1 || post[0] == rule.width / 2) && post[1] == 1 && board[rule.width / 2 - 1][0] > rule.classnum) {
                if (rule.unit[board[pro[0]][pro[1]]].strength[board[rule.width / 2 - 1][0] - rule.classnum] == 1) {
                    board[post[0]][post[1]] = board[pro[0]][pro[1]];

                } else if (rule.unit[board[pro[0]][pro[1]]].strength[board[rule.width / 2 - 1][0] - rule.classnum] == 0) {
                    board[post[0]][post[1]] = 0;

                }
            } else if (board[post[0]][post[1]] == 16 + rule.classnum && post[1] > 0 && board[post[0]][post[1] - 1] > rule.classnum) {
                if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1] - 1] - rule.classnum] == 1) {
                    board[post[0]][post[1]] = board[pro[0]][pro[1]];

                } else if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1] - 1] - rule.classnum] == 0) {
                    board[post[0]][post[1]] = 0;

                }
            } else {
                if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]] - rule.classnum] == 1) {
                    board[post[0]][post[1]] = board[pro[0]][pro[1]];

                } else if (rule.unit[board[pro[0]][pro[1]]].strength[board[post[0]][post[1]] - rule.classnum] == 0) {
                    board[post[0]][post[1]] = 0;

                }
            }
        } else if (board[post[0]][post[1]] > 0 && board[post[0]][post[1]] <= rule.classnum) {
            if (board[post[0]][post[1]] == 16 && (post[0] == rule.width / 2 - 1 || post[0] == rule.width / 2) && post[1] == rule.height - 2 && board[rule.width / 2][rule.height - 1] > 0 && board[rule.width / 2][rule.height - 1] <= rule.classnum) {
                if (rule.unit[board[pro[0]][pro[1]] - rule.classnum].strength[board[rule.width / 2][rule.height - 1]] == 1) {
                    board[post[0]][post[1]] = board[pro[0]][pro[1]];

                } else if (rule.unit[board[pro[0]][pro[1]] - rule.classnum].strength[board[rule.width / 2][rule.height - 1]] == 0) {
                    board[post[0]][post[1]] = 0;

                }
            } else if (board[post[0]][post[1]] == 16 && post[1] < rule.height - 1 && board[post[0]][post[1] + 1] > 0 && board[post[0]][post[1] + 1] <= rule.classnum) {
                if (rule.unit[board[pro[0]][pro[1]] - rule.classnum].strength[board[post[0]][post[1] + 1]] == 1) {
                    board[post[0]][post[1]] = board[pro[0]][pro[1]];

                } else if (rule.unit[board[pro[0]][pro[1]] - rule.classnum].strength[board[post[0]][post[1] + 1]] == 0) {
                    board[post[0]][post[1]] = 0;

                }
            } else {
                if (rule.unit[board[pro[0]][pro[1]] - rule.classnum].strength[board[post[0]][post[1]]] == 1) {
                    board[post[0]][post[1]] = board[pro[0]][pro[1]];

                } else if (rule.unit[board[pro[0]][pro[1]] - rule.classnum].strength[board[post[0]][post[1]]] == 0) {
                    board[post[0]][post[1]] = 0;

                }
            }
        } else {
            board[post[0]][post[1]] = board[pro[0]][pro[1]];

        }
        board[pro[0]][pro[1]] = 0;

        return;
    }


    module.exports = Kyokumen;


    
