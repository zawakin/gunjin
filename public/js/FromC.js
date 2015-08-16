

inirule.classnum = 16;
inirule.height = 8;
inirule.width = 6;

for (var i = 0; i < inirule.classnum; i++) {
    inirule.unit = [];
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
strcpy(inirule.unit[4].name, "TSa");

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
