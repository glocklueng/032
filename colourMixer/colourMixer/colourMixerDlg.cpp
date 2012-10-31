
// colourMixerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "colourMixer.h"
#include "colourMixerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

paletteEntry fullPalette[] = {
   {
    "0001375",
    "Titanate Yellow",
    143.83,
    rgb(253,240,113),
    rgb(253,240,113),
    rgb(253,231,113),
    rgb(253,231,95),
    rgb(250,214,95),
    rgb(250,214,95),
    "ag"
  },
 {
    "0001135",
    "Cadmium Yellow Primrose",
    157.6,
    rgb(249,240,64),
    rgb(253,240,82),
    rgb(253,239,82),
    rgb(253,239,72),
    rgb(253,234,65),
    rgb(253,207,51),
    "ac"
  },
 {
    "0001180",
    "Hansa Yellow Light",
    23.6,
    rgb(255,224,26),
    rgb(255,214,26),
    rgb(255,201,26),
    rgb(255,199,26),
    rgb(255,171,17),
    rgb(255,171,9),
    "abcd"
  },
 {
    "0001007",
    "Bismuth Vanadate Yellow",
    24.4,
    rgb(255,213,0),
    rgb(255,207,36),
    rgb(255,205,36),
    rgb(255,205,27),
    rgb(255,179,19),
    rgb(255,184,15),
    "acf"
  },
 {
    "0001120",
    "Cadmium Yellow Light",
    85.400,
    rgb(246,219,0),
    rgb(248,219,0),
    rgb(250,219,0),
    rgb(248,231,38),
    rgb(248,231,32),
    rgb(250,232,25),
    "ae"
  },
 {
    "0001530",
    "Primary Yellow",
    95.17,
    rgb(255,202,3),
    rgb(255,202,3),
    rgb(255,202,3),
    rgb(255,202,3),
    rgb(255,202,3),
    rgb(255,202,3),
    "abg"
  },
 {
    "0001191",
    "Hansa Yellow Opaque",
    92.91,
    rgb(255,191,0),
    rgb(255,191,0),
    rgb(255,171,23),
    rgb(255,157,23),
    rgb(255,148,27),
    rgb(255,105,17),
    "abcf"
  },
 {
    "0001130",
    "Cad Yellow Medium",
    90,
    rgb(248,180,9),
    rgb(248,173,9),
    rgb(248,162,9),
    rgb(248,147,9),
    rgb(253,138,26),
    rgb(193,96,17),
    "ac"
  },
 {
    "0001554",
    "Cadmium Yellow Medium Hue",
    70.3,
    rgb(245,173,0),
    rgb(245,170,48),
    rgb(237,159,39),
    rgb(237,149,39),
    rgb(237,131,33),
    rgb(165,81,21),
    "ab"
  },
 {
    "0001190",
    "Hansa Yellow Medium",
    30.4,
    rgb(255,183,0),
    rgb(235,170,24),
    rgb(235,163,25),
    rgb(232,143,19),
    rgb(246,120,9),
    rgb(228,80,9),
    "abcd"
  },
 {
    "0001463",
    "Aureolin Hue",
    77.36,
    rgb(186,124,27),
    rgb(186,124,27),
    rgb(186,124,27),
    rgb(186,115,27),
    rgb(186,111,27),
    rgb(192,97,18),
    "ac"
  },
 {
    "0001225",
    "Nickel Azo Yellow",
    79.6,
    rgb(132,72,13),
    rgb(132,88,13),
    rgb(169,106,18),
    rgb(169,118,18),
    rgb(177,106,18),
    rgb(201,106,18),
    "abc"
  },
 {
    "0001455",
    "Indian Yellow Hue",
    18.860,
    rgb(193,78,12),
    rgb(169,78,12),
    rgb(169,78,12),
    rgb(134,63,10),
    rgb(142,53,13),
    rgb(135,58,12),
    "abce"
  },
 {
    "0001280",
    "Quin Burnt Orange",
    40.4,
    rgb(68,14,22),
    rgb(100,14,13),
    rgb(89,13,14),
    rgb(72,15,14),
    rgb(65,14,13),
    rgb(58,12,9),
    "abc"
  },
 {
    "0001301",
    "Quin Nickel Azo",
    27.6,
    rgb(97,26,0),
    rgb(109,35,0),
    rgb(109,35,0),
    rgb(133,47,12),
    rgb(148,43,11),
    rgb(145,32,7),
    "abc"
  },
 {
    "0001110",
    "Cadmium Yellow Dark",
    121.6,
    rgb(246,148,0),
    rgb(246,139,0),
    rgb(246,128,0),
    rgb(242,122,19),
    rgb(242,108,19),
    rgb(242,91,19),
    "ac"
  },
 {
    "0001147",
    "Diarylide Yellow",
    94.4,
    rgb(255,136,19),
    rgb(255,130,30),
    rgb(255,118,29),
    rgb(255,102,22),
    rgb(255,81,19),
    rgb(255,52,11),
    "abcf"
  },
 {
    "0001070",
    "Cadmium Orange",
    111.2,
    rgb(250,71,0),
    rgb(250,72,17),
    rgb(250,67,24),
    rgb(250,59,24),
    rgb(250,49,24),
    rgb(250,50,23),
    "ac"
  },
 {
    "0001403",
    "Vat Orange",
    54.4,
    rgb(225,54,24),
    rgb(225,49,24),
    rgb(225,44,24),
    rgb(225,34,18),
    rgb(225,29,18),
    rgb(255,17,11),
    "ab"
  },
 {
    "0001384",
    "Transparent Pyrrole Orange",
    32,
    rgb(240,21,0),
    rgb(205,40,0),
    rgb(169,33,8),
    rgb(132,25,10),
    rgb(132,24,10),
    rgb(135,24,10),
    "ab"
  },
  {
    "0001276",
    "Pyrrole Orange",
    26.69,
    rgb(244,38,12),
    rgb(244,30,14),
    rgb(226,28,18),
    rgb(191,19,13),
    rgb(174,15,12),
    rgb(142,13,9),
    "abc"
  },
  {
    "0001090",
    "Cadmium Red Light",
    23.500,
    rgb(203,21,26),
    rgb(203,24,36),
    rgb(203,24,27),
    rgb(198,36,37),
    rgb(198,35,40),
    rgb(198,35,40),
    "ace"
  },
  {
    "0001279",
    "Pyrrole Red Light",
    78.4,
    rgb(203,0,27),
    rgb(193,0,25),
    rgb(183,16,25),
    rgb(176,26,28),
    rgb(176,27,24),
    rgb(198,23,17),
    "ab"
  },
  {
    "0001210",
    "Naphthol Red Light",
    86.4,
    rgb(200,24,30),
    rgb(178,13,26),
    rgb(178,9,26),
    rgb(159,11,23),
    rgb(145,10,17),
    rgb(134,15,17),
    "abcd"
  },
  {
    "0001277",
    "Pyrrole Red",
    88.4,
    rgb(182,1,30),
    rgb(170,3,28),
    rgb(163,3,22),
    rgb(136,14,22),
    rgb(136,20,22),
    rgb(136,20,18),
    "abc"
  },
  {
    "0001100",
    "Cadmium Red Medium",
    50.8,
    rgb(171,11,28),
    rgb(153,6,23),
    rgb(131,18,23),
    rgb(127,22,22),
    rgb(120,20,19),
    rgb(135,19,16),
    "ac"
  },
  {
    "0001552",
    "Cadmium Red Medium Hue",
    78.4,
    rgb(165,11,26),
    rgb(155,10,24),
    rgb(138,14,27),
    rgb(168,17,31),
    rgb(146,12,22),
    rgb(188,17,21),
    "ab"
  },
  {
    "0001320",
    "Quin Red Light",
    38,
    rgb(166,24,36),
    rgb(166,21,36),
    rgb(166,26,39),
    rgb(147,22,32),
    rgb(147,22,25),
    rgb(153,15,13),
    "a"
  },
  {
    "0001080",
    "Cadmium Red Dark",
    47.6,
    rgb(134,4,36),
    rgb(138,15,36),
    rgb(110,18,25),
    rgb(93,20,24),
    rgb(86,16,17),
    rgb(106,24,19),
    "ac"
  },
  {
    "0001278",
    "Pyrrole Red Dark",
    58.8,
    rgb(145,0,29),
    rgb(138,0,37),
    rgb(125,0,27),
    rgb(106,10,24),
    rgb(92,11,18),
    rgb(80,13,13),
    "ac"
  },
  {
    "0001510",
    "Primary Magenta",
    36.4,
    rgb(160,15,35),
    rgb(160,15,35),
    rgb(149,10,41),
    rgb(120,17,34),
    rgb(120,13,23),
    rgb(120,13,16),
    "ab"
  },
  {
    "0001562",
    "Light Magenta",
    150,
    rgb(255,108,125),
    rgb(255,108,125),
    rgb(244,103,120),
    rgb(206,88,92),
    rgb(214,81,74),
    rgb(214,68,61),
    "a"
  },
  {
    "0001220",
    "Naphthol Red Medium",
    82.4,
    rgb(141,1,22),
    rgb(127,1,29),
    rgb(127,1,28),
    rgb(124,10,28),
    rgb(122,16,24),
    rgb(122,16,19),
    "abc"
  },
  {
    "0001310",
    "Quin Red",
    79.2,
    rgb(138,13,37),
    rgb(128,6,43),
    rgb(128,6,38),
    rgb(106,9,34),
    rgb(104,16,29),
    rgb(104,16,21),
    "abcf"
  },
  {
    "0001450",
    "Alizarin Crimson Hue",
    10.400,
    rgb(44,11,14),
    rgb(44,11,14),
    rgb(44,11,14),
    rgb(44,11,14),
    rgb(44,11,12),
    rgb(89,12,10),
    "abce"
  },
  {
    "0001290",
    "Quin Crimson",
    72.4,
    rgb(49,6,17),
    rgb(63,6,17),
    rgb(65,11,20),
    rgb(74,17,24),
    rgb(66,15,19),
    rgb(99,17,17),
    "abc"
  },
  {
    "0001252",
    "Permanent Maroon",
    60,
    rgb(29,0,16),
    rgb(43,4,16),
    rgb(51,10,21),
    rgb(51,14,21),
    rgb(53,17,20),
    rgb(53,17,17),
    "ac"
  },
  {
    "0001305",
    "Quin Magenta",
    35.8,
    rgb(94,0,42),
    rgb(109,0,53),
    rgb(109,0,47),
    rgb(109,13,42),
    rgb(109,18,34),
    rgb(109,19,27),
    "abcd"
  },
  {
    "0001570",
    "Medium Magenta",
    88.4,
    rgb(173,35,97),
    rgb(173,35,106),
    rgb(161,33,91),
    rgb(146,30,69),
    rgb(132,27,50),
    rgb(135,24,32),
    "a"
  },
  {
    "0001330",
    "Quin Violet",
    70.65,
    rgb(83,5,26),
    rgb(83,5,38),
    rgb(79,17,43),
    rgb(68,19,40),
    rgb(61,17,36),
    rgb(46,16,29),
    "ab"
  },
  {
    "0001253",
    "Permanent Violet Dark",
    36.4,
    rgb(30,9,24),
    rgb(31,5,33),
    rgb(30,5,26),
    rgb(30,9,21),
    rgb(27,11,19),
    rgb(32,9,16),
    "abc"
  },
  {
    "0001465",
    "Cobalt Violet Hue",
    42,
    rgb(49,14,32),
    rgb(49,14,40),
    rgb(50,13,38),
    rgb(47,19,36),
    rgb(46,19,31),
    rgb(37,14,18),
    "a"
  },
  {
    "0001572",
    "Medium Violet",
    119.6,
    rgb(57,28,66),
    rgb(64,28,70),
    rgb(61,27,63),
    rgb(52,25,49),
    rgb(60,25,49),
    rgb(55,20,39),
    "a"
  },
  {
    "0001568",
    "Light Violet",
    134.4,
    rgb(88,65,109),
    rgb(80,59,99),
    rgb(69,50,82),
    rgb(75,50,82),
    rgb(59,37,61),
    rgb(53,33,38),
    "a"
  },
  {
    "0001401",
    "Ultramarine Violet",
    9.2,
    rgb(33,19,50),
    rgb(40,26,52),
    rgb(39,24,40),
    rgb(38,22,34),
    rgb(29,16,18),
    rgb(35,16,14),
    "abc"
  },
  {
    "0001150",
    "Dioxazine Purple",
    15.6,
    rgb(10,5,14),
    rgb(10,5,19),
    rgb(9,4,19),
    rgb(8,4,15),
    rgb(6,3,10),
    rgb(6,3,6),
    "abc"
  },
  {
    "0001467",
    "Smalt Hue",
    0.69,
    rgb(11,11,36),
    rgb(9,13,30),
    rgb(6,8,14),
    rgb(4,5,9),
    rgb(3,4,4),
    rgb(3,6,4),
    "ag"
  },
  {
    "0001400",
    "Ultramarine Blue",
    2.500,
    rgb(19,5,61),
    rgb(8,12,56),
    rgb(8,12,40),
    rgb(6,9,26),
    rgb(6,8,17),
    rgb(5,8,12),
    "abceg"
  },
  {
    "0001566",
    "Light Ultramarine Blue",
    67.3,
    rgb(94,116,164),
    rgb(94,116,164),
    rgb(71,88,141),
    rgb(44,62,101),
    rgb(35,51,76),
    rgb(29,37,49),
    "acg"
  },
  {
    "0001140",
    "Cobalt Blue",
    32.8,
    rgb(0,25,110),
    rgb(0,39,92),
    rgb(15,41,80),
    rgb(18,35,66),
    rgb(20,33,46),
    rgb(23,42,53),
    "abcf"
  },
  {
    "0001556",
    "Cobalt Blue Hue",
    115.6,
    rgb(2,39,92),
    rgb(6,38,88),
    rgb(5,41,95),
    rgb(5,39,73),
    rgb(8,33,52),
    rgb(12,34,46),
    "ag"
  },
  {
    "0001050",
    "Cerulean Blue Chromium",
    34,
    rgb(5,62,90),
    rgb(5,62,92),
    rgb(16,54,75),
    rgb(24,47,58),
    rgb(20,35,38),
    rgb(19,31,29),
    "abcf"
  },
  {
    "0001005",
    "Anthraquinone Blue",
    15.4,
    rgb(12,11,20),
    rgb(8,11,47),
    rgb(5,15,42),
    rgb(5,15,42),
    rgb(5,15,42),
    rgb(5,15,42),
    "abcd"
  },
  {
    "0001460",
    "Prussian Blue Hue",
    2.000,
    rgb(3,8,14),
    rgb(3,7,13),
    rgb(1,11,19),
    rgb(1,7,13),
    rgb(1,5,8),
    rgb(1,3,4),
    "abce"
  },
  {
    "0001260",
    "Phthalo Blue RS",
    100.8,
    rgb(6,1,30),
    rgb(4,27,73),
    rgb(0,37,83),
    rgb(0,34,62),
    rgb(0,26,47),
    rgb(6,24,33),
    "abcf"
  },
  {
    "0001255",
    "Phthalo Blue GS",
    67.6,
    rgb(1,2,28),
    rgb(0,50,84),
    rgb(0,55,80),
    rgb(0,55,80),
    rgb(4,40,70),
    rgb(3,33,52),
    "abcdf"
  },
  {
    "0001500",
    "Primary Cyan",
    42,
    rgb(8,47,78),
    rgb(6,52,86),
    rgb(0,50,78),
    rgb(0,41,63),
    rgb(0,28,43),
    rgb(5,23,30),
    "abfg"
  },
  {
    "0001464",
    "Azurite Hue",
    30.4,
    rgb(18,34,47),
    rgb(12,49,61),
    rgb(9,45,53),
    rgb(16,36,41),
    rgb(16,32,34),
    rgb(12,23,23),
    "ag"
  },
  {
    "0001051",
    "Cerulean Blue Deep",
    31.6,
    rgb(0,62,83),
    rgb(0,62,83),
    rgb(0,36,44),
    rgb(4,29,37),
    rgb(10,23,27),
    rgb(10,23,20),
    "abf"
  },
  {
    "0001457",
    "Manganese Blue Hue",
    10,
    rgb(0,59,89),
    rgb(0,62,86),
    rgb(0,43,59),
    rgb(2,29,38),
    rgb(4,20,27),
    rgb(3,11,13),
    "abcfg"
  },
  {
    "0001390",
    "Turquoise",
    88,
    rgb(0,10,8),
    rgb(0,92,86),
    rgb(0,76,71),
    rgb(0,51,47),
    rgb(0,42,39),
    rgb(0,28,24),
    "abf"
  },
  {
    "0001144",
    "Cobalt Turquoise",
    55.2,
    rgb(1,76,73),
    rgb(0,77,72),
    rgb(0,55,47),
    rgb(8,49,41),
    rgb(17,36,30),
    rgb(14,28,19),
    "abcf"
  },
  {
    "0001145",
    "Teal",
    23.6,
    rgb(7,131,122),
    rgb(7,125,117),
    rgb(15,117,103),
    rgb(21,82,72),
    rgb(11,47,38),
    rgb(9,55,31),
    "abcf"
  },
  {
    "0001564",
    "Light Turquois (Phthalo)",
    45.3,
    rgb(0,118,93),
    rgb(5,91,73),
    rgb(4,75,60),
    rgb(3,53,37),
    rgb(6,40,26),
    rgb(6,40,19),
    "a"
  },
  {
    "0001270",
    "Phthalo Green BS",
    0.75,
    rgb(0,5,5),
    rgb(0,34,23),
    rgb(0,25,16),
    rgb(0,13,8),
    rgb(0,9,4),
    rgb(0,7,2),
    "abcdf"
  },
  {
    "0001469",
    "Viridian Green Hue",
    15.200,
    rgb(0,52,39),
    rgb(0,81,66),
    rgb(6,73,59),
    rgb(6,64,47),
    rgb(21,53,37),
    rgb(17,44,28),
    "abceg"
  },
  {
    "0001275",
    "Phthalo Green YS",
    130,
    rgb(0,22,11),
    rgb(0,100,60),
    rgb(0,96,53),
    rgb(0,80,44),
    rgb(0,80,33),
    rgb(13,78,32),
    "abcf"
  },
  {
    "0001250",
    "Perm Green Light",
    97.6,
    rgb(0,83,42),
    rgb(0,114,57),
    rgb(0,90,45),
    rgb(0,70,32),
    rgb(0,67,25),
    rgb(13,49,19),
    "a"
  },
  {
    "0001142",
    "Cobalt Green",
    114,
    rgb(16,46,31),
    rgb(20,56,43),
    rgb(31,56,45),
    rgb(33,54,43),
    rgb(39,53,38),
    rgb(34,43,31),
    "acf"
  },
  {
    "0001454",
    "Hookers Green Hue",
    80.4,
    rgb(4,9,5),
    rgb(10,28,21),
    rgb(8,28,17),
    rgb(15,26,17),
    rgb(18,24,15),
    rgb(18,24,15),
    "a"
  },
  {
    "0001195",
    "Jenkins Green",
    19.2,
    rgb(6,13,7),
    rgb(18,35,23),
    rgb(17,28,16),
    rgb(13,24,13),
    rgb(6,9,5),
    rgb(4,6,3),
    "abc"
  },
  {
    "0001461",
    "Sap Green Hue",
    6,
    rgb(13,15,7),
    rgb(13,15,7),
    rgb(11,14,6),
    rgb(9,9,4),
    rgb(6,6,3),
    rgb(4,4,2),
    "abc"
  },
  {
    "0001468",
    "Terre Verte Hue",
    11.2,
    rgb(39,43,26),
    rgb(39,43,26),
    rgb(39,40,26),
    rgb(36,34,22),
    rgb(21,20,13),
    rgb(10,10,5),
    "acfg"
  },
  {
    "0001061",
    "Chromium Oxide Dark",
    106,
    rgb(30,45,21),
    rgb(29,42,27),
    rgb(46,58,44),
    rgb(51,58,44),
    rgb(56,58,44),
    rgb(33,34,26),
    "acf"
  },
  {
    "0001060",
    "Chromium Oxide",
    12.19,
    rgb(47,71,34),
    rgb(47,71,34),
    rgb(47,71,34),
    rgb(47,71,34),
    rgb(47,71,34),
    rgb(47,71,34),
    "acf"
  },
  {
    "0001143",
    "Cobalt Titanate Green",
    24.27,
    rgb(68,131,66),
    rgb(68,131,66),
    rgb(68,131,66),
    rgb(68,131,66),
    rgb(68,131,66),
    rgb(68,131,66),
    "af"
  },
  {
    "0001558",
    "Light Green/B.S.",
    27.6,
    rgb(45,150,58),
    rgb(42,155,54),
    rgb(35,136,30),
    rgb(26,99,23),
    rgb(15,58,13),
    rgb(9,33,7),
    "a"
  },
  {
    "0001560",
    "Light Green/Y.S.",
    25.2,
    rgb(77,150,39),
    rgb(70,136,35),
    rgb(54,124,27),
    rgb(39,89,19),
    rgb(28,50,11),
    rgb(18,44,6),
    "a"
  },
  {
    "0001170",
    "Green Gold",
    28.4,
    rgb(58,67,11),
    rgb(82,77,10),
    rgb(77,72,9),
    rgb(72,67,8),
    rgb(62,50,7),
    rgb(43,35,5),
    "abc"
  },
  {
    "0001459",
    "Naples Yellow Hue",
    11.6,
    rgb(222,158,90),
    rgb(175,120,61),
    rgb(127,80,39),
    rgb(103,54,20),
    rgb(82,34,11),
    rgb(41,17,5),
    "abcfg"
  },
  {
    "0001410",
    "Yellow Oxide",
    27.6,
    rgb(179,117,42),
    rgb(151,94,35),
    rgb(120,72,28),
    rgb(115,64,23),
    rgb(76,36,13),
    rgb(50,20,7),
    "abcfg"
  },
  {
    "0001407",
    "Yellow Ochre",
    32.4,
    rgb(160,96,44),
    rgb(160,96,44),
    rgb(136,81,37),
    rgb(120,68,33),
    rgb(64,34,16),
    rgb(45,23,10),
    "abcg"
  },
  {
    "0001386",
    "Transparent Yellow Iron Oxide",
    21.6,
    rgb(117,65,14),
    rgb(117,65,14),
    rgb(116,58,13),
    rgb(106,54,17),
    rgb(110,52,17),
    rgb(146,56,11),
    "abc"
  },
  {
    "0001340",
    "Raw Sienna",
    46.08,
    rgb(132,77,32),
    rgb(132,77,32),
    rgb(132,77,32),
    rgb(132,77,32),
    rgb(132,77,32),
    rgb(132,77,32),
    "abcfg"
  },
  {
    "0001202",
    "Mars yellow",
    32,
    rgb(131,60,30),
    rgb(118,52,30),
    rgb(92,35,20),
    rgb(82,28,16),
    rgb(62,18,9),
    rgb(42,12,6),
    "acfg"
  },
  {
    "0001385",
    "Transparent Red Iron Oxide",
    15.51,
    rgb(69,18,0),
    rgb(62,16,7),
    rgb(75,24,12),
    rgb(69,20,15),
    rgb(64,21,14),
    rgb(66,17,9),
    "abc"
  },
  {
    "0001360",
    "Red Oxide",
    45.2,
    rgb(110,27,4),
    rgb(92,22,18),
    rgb(69,16,13),
    rgb(57,16,12),
    rgb(51,14,11),
    rgb(77,14,9),
    "abcfg"
  },
  {
    "0001405",
    "Violet Oxide",
    52,
    rgb(80,28,28),
    rgb(54,22,22),
    rgb(51,23,21),
    rgb(45,22,20),
    rgb(38,23,19),
    rgb(36,21,16),
    "abcfg"
  },
  {
    "0001020",
    "Burnt Sienna",
    26.56,
    rgb(87,44,32),
    rgb(87,44,32),
    rgb(87,44,32),
    rgb(87,44,32),
    rgb(87,44,32),
    rgb(87,44,32),
    "abcfg"
  },
  {
    "0001383",
    "Transparent Brown Iron Oxide",
    10.4,
    rgb(34,13,0),
    rgb(40,15,0),
    rgb(34,12,3),
    rgb(32,12,5),
    rgb(23,8,4),
    rgb(15,6,3),
    "ac"
  },
  {
    "0001035",
    "Burnt Umber Light",
    22,
    rgb(48,27,17),
    rgb(41,23,14),
    rgb(34,18,10),
    rgb(29,17,11),
    rgb(20,12,7),
    rgb(15,9,5),
    "abg"
  },
  {
    "0001030",
    "Burnt Umber",
    25.5,
    rgb(37,24,17),
    rgb(37,24,17),
    rgb(37,24,17),
    rgb(27,17,11),
    rgb(22,14,9),
    rgb(16,11,7),
    "acg"
  },
  {
    "0001350",
    "Raw Umber",
    50.39,
    rgb(40,30,18),
    rgb(40,30,18),
    rgb(40,30,18),
    rgb(40,30,18),
    rgb(40,30,18),
    rgb(40,30,18),
    "abcfg"
  },
  {
    "0001462",
    "Van Dyke Brown  Hue",
    5.36,
    rgb(12,8,5),
    rgb(10,7,4),
    rgb(9,6,4),
    rgb(8,5,3),
    rgb(4,3,2),
    rgb(3,2,1),
    "abcf"
  },
  {
    "0001040",
    "Carbon Black",
    74.8,
    rgb(7,9,8),
    rgb(6,5,7),
    rgb(6,6,6),
    rgb(8,8,7),
    rgb(8,8,8),
    rgb(6,7,6),
    "abcfg"
  },
  {
    "0001200",
    "Mars Black",
    12.4,
    rgb(12,12,12),
    rgb(12,14,12),
    rgb(14,13,12),
    rgb(13,13,12),
    rgb(10,9,8),
    rgb(8,8,6),
    "afg"
  },
  {
    "0001010",
    "Bone Black",
    3.6,
    rgb(8,8,8),
    rgb(7,7,7),
    rgb(7,7,7),
    rgb(8,7,6),
    rgb(5,5,4),
    rgb(3,3,2),
    "abcfg"
  },
  {
    "0001160",
    "Graphite Gray",
    1.6,
    rgb(63,56,46),
    rgb(19,18,17),
    rgb(7,7,7),
    rgb(3,3,3),
    rgb(2,2,2),
    rgb(4,4,2),
    "afg"
  },
  {
    "0001240",
    "Paynes Gray",
    12,
    rgb(8,11,14),
    rgb(17,21,29),
    rgb(13,19,29),
    rgb(7,10,15),
    rgb(7,9,11),
    rgb(6,8,9),
    "abcg"
  },
  {
    "0001370",
    "Titan Buff",
    35.6,
    rgb(215,188,146),
    rgb(215,191,146),
    rgb(211,189,143),
    rgb(169,155,111),
    rgb(127,115,75),
    rgb(80,71,38),
    "abcfg"
  },
  {
    "0001415",
    "Zinc White",
    12.5,
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    "abcfg"
  },
  {
    "0001380",
    "Titanium White",
    99.99,
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    rgb(250,250,247),
    "abcdefg"
  },
  {
    "0001442",
    "Neutral Gray N2",
    27.2,
    rgb(24,22,19),
    rgb(24,22,19),
    rgb(24,22,19),
    rgb(24,22,19),
    rgb(24,22,19),
    rgb(24,22,19),
    "afg"
  },
  {
    "0001443",
    "Neutral Gray N3",
    27.2,
    rgb(30,27,23),
    rgb(30,27,23),
    rgb(30,27,23),
    rgb(30,27,23),
    rgb(30,27,23),
    rgb(30,27,23),
    "afg"
  },
  {
    "0001444",
    "Neutral Gray N4",
    27.2,
    rgb(38,35,29),
    rgb(38,35,29),
    rgb(38,35,29),
    rgb(38,35,29),
    rgb(38,35,29),
    rgb(38,35,29),
    "afg"
  },
  {
    "0001445",
    "Neutral Gray N5",
    27.2,
    rgb(51,46,40),
    rgb(51,46,40),
    rgb(51,46,40),
    rgb(51,46,40),
    rgb(51,46,40),
    rgb(51,46,40),
    "acfg"
  },
  {
    "0001446",
    "Neutral Gray N6",
    27.2,
    rgb(65,65,60),
    rgb(65,65,60),
    rgb(65,65,60),
    rgb(65,65,60),
    rgb(65,65,60),
    rgb(65,65,60),
    "afg"
  },
  {
    "0001447",
    "Neutral Gray N7",
    27.2,
    rgb(101,94,89),
    rgb(101,94,89),
    rgb(101,94,89),
    rgb(101,94,89),
    rgb(101,94,89),
    rgb(101,94,89),
    "afg"
  },
  {
    "0001448",
    "Neutral Gray N8",
    27.2,
    rgb(118,108,101),
    rgb(118,108,101),
    rgb(118,108,101),
    rgb(118,108,101),
    rgb(118,108,101),
    rgb(118,108,101),
    "afg"
  }
};



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcolourMixerDlg dialog




CcolourMixerDlg::CcolourMixerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcolourMixerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcolourMixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_CButtonB);
	DDX_Control(pDX, IDC_SLIDER1, m_MixAmount_A);
}

BEGIN_MESSAGE_MAP(CcolourMixerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CcolourMixerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CcolourMixerDlg message handlers

BOOL CcolourMixerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcolourMixerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcolourMixerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcolourMixerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcolourMixerDlg::OnBnClickedButton1()
{	
	CWnd* pWnd = this;

	CMFCColorDialog ColorDlg(0,0,pWnd);
	
	ColorDlg.SetCurrentColor(RGB(0,255,0));
	ColorDlg.SetNewColor(RGB(0,0,255));
	// set the red, green, and blue components of a selected 
	// color on the two property pages of the color dialog
	ColorDlg.SetPageOne(255,0,0);
	ColorDlg.SetPageTwo(0,255,0);

	if (IDOK == ColorDlg.DoModal())
	{
	}
}
