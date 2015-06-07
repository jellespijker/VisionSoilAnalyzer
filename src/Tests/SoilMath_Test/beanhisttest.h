/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */



#pragma once
#include <stdint.h>
#include <cstdlib>

using namespace std;

// Data from beans.ppm

const uint32_t actualHistBean[256]  = {0,0,58,365,639,876,1196,1270,1373,1754,1665,1630,1802,1717,1921,1754,1699,1844,1721,1668,1639,1587,1611,1520,1508,1483,1497,1473,1395,1367,1398,1310,1282,1223,1258,1306,1289,1293,1275,1231,1297,1233,1299,1280,1276,1318,1325,1306,1352,1392,1348,1324,1385,1426,1415,1427,1403,1418,1412,1573,1454,1448,1463,1566,1491,1470,1517,1537,1484,1487,1492,1485,1507,1504,1516,1490,1470,1486,1504,1490,1482,1478,1498,1423,1384,1447,1417,1453,1428,1343,1378,1504,1376,1426,1369,1420,1431,1362,1359,1366,1410,1349,1400,1420,1369,1427,1415,1371,1442,1373,1390,1363,1412,1382,1358,1371,1383,1294,1423,1381,1417,1345,1308,1386,1315,1378,1352,1377,1315,1323,1379,1409,1306,1324,1331,1360,1320,1318,1282,1269,1317,1285,1307,1311,1347,1354,1339,1320,1341,1304,1280,1341,1281,1237,1294,1259,1312,1228,1239,1283,1202,1200,1270,1247,1177,1247,1165,1213,1177,1145,1115,1144,1137,1089,1075,1091,1147,1065,1140,1103,1027,1131,1080,1107,1064,1029,1026,1038,988,992,1043,979,988,972,1012,919,959,966,909,872,902,852,822,846,827,799,761,722,725,665,692,659,635,596,593,602,572,570,506,552,499,479,487,434,410,419,321,328,334,291,207,208,187,151,147,121,106,74,75,60,63,36,26,27,19,18,15,23,15,13,10,9,3,0,0,0};
const float actualMeanBean = 104.263387784091;
const uchar actualminBean = 2;
const uchar actualMaxBean = 252;
const float actualStdBean = 61.106823;
const uint32_t actualSumBean = 29360570;

const uint32_t actualHistBeanMask[256] = {0,0,30,156,238,363,484,509,534,695,651,653,745,708,741,676,611,644,648,600,596,590,588,526,559,528,531,556,486,494,524,476,454,475,469,491,455,467,454,457,495,444,463,486,490,521,490,500,522,554,514,531,528,555,571,533,535,550,522,608,582,596,537,616,577,577,611,599,544,570,592,560,560,565,589,575,522,562,591,575,587,561,578,547,539,587,569,582,569,523,541,626,538,597,521,542,557,516,523,505,525,531,528,560,531,539,539,522,537,510,508,525,530,523,511,504,527,495,503,507,539,511,495,507,502,505,506,545,482,553,525,550,502,511,537,524,491,507,531,461,510,493,507,506,486,477,528,505,533,492,444,486,520,498,468,462,488,491,493,484,481,434,477,449,433,460,429,483,430,462,455,465,442,410,435,440,420,407,487,423,407,431,393,448,424,378,397,392,418,394,408,391,394,376,365,354,354,366,381,347,352,344,312,308,329,312,291,288,299,262,292,255,267,229,276,268,240,261,230,253,216,223,201,175,171,171,127,122,137,102,86,94,65,63,57,59,41,37,34,24,25,14,12,12,11,8,5,15,8,10,8,5,0,0,0,0};
const float acutalMeanBeanMask = 105.113610746688;
const uint32_t actualSumBeanMask = 11377287;
const uchar actualminBeanMask = 2;
const uchar actualMaxBeanMask = 251;
const float actualStdBeanMask = 61.4932556;


