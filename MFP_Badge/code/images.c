#include "optiLoader.h"

image_t PROGMEM image_328 = {
    {"adaboot_atmega328.hex"},
    {"atmega328"},
    0x950F,				/* Signature bytes for 328P */
    {0x3F, 0xFF, 0xDA, 0x05},            // pre program fuses (prot/lock, low, high, ext)
    {0x0F, 0x0, 0x0, 0x0},            // post program fuses
    {0x3F, 0xFF, 0xFF, 0x07},           // fuse mask
    32768,     // size of chip flash in bytes
    128,   // size in bytes of flash page
    {
    ":207800000C94343C0C94513C0C94513C0C94513C0C94513C0C94513C0C94513C0C94513C1D\n"
	":207820000C94513C0C94513C0C94513C0C94513C0C94513C0C94513C0C94513C0C94513CE0\n"
	":207840000C94513C0C94513C0C94513C0C94513C0C94513C0C94513C0C94513C0C94513CC0\n"
	":207860000C94513C0C94513C11241FBECFEFD8E0DEBFCDBF11E0A0E0B1E0E8EEFFE702C07D\n"
	":2078800005900D92A230B107D9F712E0A2E0B1E001C01D92AD30B107E1F70E94343D0C94C5\n"
	":2078A000F23F0C94003C982F9595959595959595905D8F708A307CF0282F295A8091C0008F\n"
	":2078C00085FFFCCF9093C6008091C00085FFFCCF2093C6000895282F205DF0CF982F8091CF\n"
	":2078E000C00085FFFCCF9093C6000895EF92FF920F931F932D98EE24FF2487018091C0003A\n"
   	":2079000087FD17C00894E11CF11C011D111D81E0E81689E0F8068DE3080780E0180770F3F3\n"
	":20792000E0910401F091050109958091C00087FFE9CF2D9A8091C6001F910F91FF90EF90A1\n"
	":2079400008950E94763C982F8091C00085FFFCCF9093C600913634F490331CF0892F80531D\n"
	":2079600008950895892F875508951F930E94A13C182F0E94A13C1295107F810F1F91089592\n"
	":20798000882351F0982F91508091C00087FFFCCF8091C6009923B9F708951F93182F0E94B6\n"
	":2079A000763C803249F0809103018F5F809303018530C1F01F9108958091C00085FFFCCF3D\n"
	":2079C00084E18093C6008091C00085FFFCCF1093C6008091C00085FFFCCF80E18093C60086\n"
	":2079E0001F910895E0910401F091050109951F9108950E94763C803241F0809103018F5F18\n"
	":207A000080930301853081F008958091C00085FFFCCF84E18093C6008091C00085FFFCCF0E\n"
	":207A200080E18093C6000895E0910401F091050109950895882309F483E040E951E02D9A0B\n"
	":207A400028EE33E0FA013197F1F721503040D1F72D9828EE33E0FA013197F1F7215030402F\n"
	":207A6000D1F7815061F708953F924F925F926F927F928F929F92AF92BF92CF92DF92EF928C\n"
	":207A8000FF920F931F93CF93DF9394B714BE809160008861809360001092600091FF7FC171\n"
	":207AA00080E18093C40088E18093C10086E08093C2005098589A259A83E00E941A3D10E031\n"
	":207AC00034E1F32E2EE1E22E95E9D92E8FE0C82E00E1B02EAA24A394B1E49B2EA6E58A2E02\n"
	":207AE000F2E57F2EE0E26E2E79E4572E63E5462E50E5352E0E94763C8033B1F1112309F494\n"
	":207B0000ADC0813309F44FC0803409F443C0813409F491C0823499F1853409F480C0803596\n"
	":207B200019F1823509F1813509F433C1853509F420C1863509F4FDC0843609F493C0843711\n"
	":207B400009F4E1C1853709F4BCC1863709F481C0809103018F5F80930301853049F011E05C\n"
	":207B60000E94763C803351F60E94F93C11E0F8CFE0910401F0910501099511E0F1CF93E168\n"
	":207B80008091C00087FFFCCF8091C600992361F39150F6CF0E94763C863830F30E94763CAD\n"
	":207BA0000E94F93CE3CF0E94763C803289F68091C00085FFFCCFF092C6008091C00085FFFA\n"
	":207BC000FCCF9092C6008091C00085FFFCCF8092C6008091C00085FFFCCF7092C600809101\n"
	":207BE000C00085FFFCCF6092C6008091C00085FFFCCF5092C6008091C00085FFFCCF409204\n"
	":207C0000C6008091C00085FFFCCF3092C6008091C00085FFFCCFB092C60011E0A1CF94E0F9\n"
	":207C20008091C00087FFFCCF8091C600992309F49BCF9150F5CF0E94763C803809F494C026\n"
	":207C4000813809F48CC0823809F49EC0883909F481C080E00E94CD3C11E082CF88E0809346\n"
	":207C60006000FFCF0E94763C809309020E94763C8093080280910C028E7F80930C020E9404\n"
	":207C8000763C853409F40DC18091080290910902892B89F000E010E00E94763CF801E85FD6\n"
	":207CA000FE4F80830F5F1F4F80910802909109020817190788F30E94763C803209F048CF81\n"
	":207CC00080910C0280FF71C02091060130910701220F331F30930701209306016091080251\n"
	":207CE0007091090261157105C9F0E8E0F1E0A901DB01AE0FBF1FF999FECF52BD41BD81919B\n"
	":207D000080BDFA9AF99A4F5F5F4FEA17FB0799F7260F371F30930701209306018091C0002F\n"
	":207D200085FFFCCFF092C6008091C00085FFFCCF72CF0E94763C803309F4A3C092E0809161\n"
	":207D4000C00087FFFCCF8091C600992309F481CF9150F5CF83E00E94CD3C11E001CF81E05D\n"
	":207D60000E94CD3C11E0FCCE82E00E94CD3C11E0F7CE0E94763C809306010E94763C809305\n"
	":207D800007010E94F93CF2CE80E10E94CD3C11E0E7CE0E94F93C88E080936000FFCFE091A1\n"
	":207DA0000401F091050109957BCE809107018823880F880B8A2180930B028091060190915E\n"
	":207DC0000701880F991F90930701809306018091080280FF09C080910802909109020196C6\n"
	":207DE0009093090280930802F894F999FECF1127E0910601F0910701C8E0D1E080910802A0\n"
	":207E000090910902103091F40091570001700130D9F303E000935700E89500915700017078\n"
	":207E20000130D9F301E100935700E895099019900091570001700130D9F301E00093570099\n"
	":207E4000E8951395103498F011270091570001700130D9F305E000935700E895009157006F\n"
	":207E600001700130D9F301E100935700E8953296029709F0C7CF103011F00296E5CF112499\n"
	":207E80004DCF0E94763C0E94763C182F0E94763C112391F0113059F08FE00E94CD3C11E039\n"
	":207EA0005FCE80910C02816080930C02EDCE85E90E94CD3C11E054CE8EE10E94CD3C11E082\n"
	":207EC0004FCE0E94763C803209F042CE8091C00085FFFCCFF092C6008091C00085FFFCCFEE\n"
	":207EE000E092C6008091C00085FFFCCFD092C6008091C00085FFFCCFC092C6008091C000F9\n"
	":207F000085FFFCCF88CE0E94763C809309020E94763C809308028091060190910701880F01\n"
	":207F2000991F90930701809306010E94763C853409F43EC080910C028E7F80930C020E944D\n"
	":207F4000763C803209F00BCE8091C00085FFFCCFF092C600A0910802B0910902109709F458\n"
	":207F60003CC080910C02182F1170082F0270E0910601F09107019F012F5F3F4FB90140E0DE\n"
	":207F800050E01123D9F4002339F494918091C00085FFFCCF9093C6004F5F5F4FCB01019673\n"
	":207FA000F9014A175B07A8F4BC012F5F3F4FE9CF80910C02816080930C02C1CFF999FECFC7\n"
	":207FC000F2BDE1BDF89A90B58091C00085FFFCCFE1CF70930701609306018091C00085FF53\n"
	":0A7FE000FCCF19CEF894FFCF80000B\n"
	":00000001FF\n"
    }
};


/*
 * Table of defined images
 */
image_t *images[] = {
  &image_328,
};

uint8_t NUMIMAGES = sizeof(images)/sizeof(images[0]);
