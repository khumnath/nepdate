/*
 * Copyright (C) 2024 khumnath
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "eventsdata.h"

const LunarEventRule lunarEvents[] = {
    {QStringLiteral("युगादि नव वर्ष"), QStringLiteral("चन्द्रमान अनुसार नयाँ वर्षको सुरुवात।"), QStringLiteral("चैत्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, false},
    {QStringLiteral("चैते दशैँ"), QStringLiteral("चैत्र महिनामा मनाइने दशैँ, ठूलो दशैँको सानो रूप।"), QStringLiteral("चैत्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("अष्टमी"), EventRule::Udaya, false},
    {QStringLiteral("रामनवमी"), QStringLiteral("भगवान रामको जन्मदिन (मध्याह्न काल)।"), QStringLiteral("चैत्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("नवमी"), EventRule::Madhyanna, true},
    {QStringLiteral("मातातीर्थ औंसी"), QStringLiteral("आमाको मुख हेर्ने दिन।"), QStringLiteral("वैशाख"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अमावस्या"), EventRule::Udaya, false},
    {QStringLiteral("घोडेजात्रा"), QStringLiteral("काठमाडौँमा मनाइने घोडे जात्रा उत्सव।"), QStringLiteral("चैत्र"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अमावस्या"), EventRule::Madhyanna, true},
    {QStringLiteral("अक्षय तृतीया"), QStringLiteral("जौ सातु खाने दिन, विवाहको लागि शुभ साइत।"), QStringLiteral("वैशाख"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("तृतीया"), EventRule::Udaya, false},
    {QStringLiteral("बुद्ध जयन्ती / उभौली पर्व"), QStringLiteral("भगवान बुद्धको जन्म र किराँत समुदायको उभौली पर्व।"), QStringLiteral("वैशाख"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Udaya, true},
    {QStringLiteral("सिथि नख:"), QStringLiteral("नेवार समुदायले इनार तथा पानीका मुहान सफा गर्ने र कुमार कार्तिकेयको पूजा गर्ने पर्व।"), QStringLiteral("ज्येष्ठ"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("षष्ठी"), EventRule::Udaya, false},
    {QStringLiteral("निर्जला एकादशी"), QStringLiteral("जल समेत नपिई व्रत बस्ने कठोर एकादशी।"), QStringLiteral("ज्येष्ठ"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("एकादशी"), EventRule::Udaya, false},
    {QStringLiteral("हरिशयनी एकादशी (तुलसी रोपण)"), QStringLiteral("चतुर्मास व्रत सुरु, घर-घरमा तुलसी रोप्ने दिन।"), QStringLiteral("आषाढ"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("एकादशी"), EventRule::Udaya, false},
    {QStringLiteral("गुरु पूर्णिमा"), QStringLiteral("गुरु र शिक्षकहरूलाई सम्मान गर्ने दिन।"), QStringLiteral("आषाढ"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Udaya, false},
    {QStringLiteral("गठाःमुगः चह्रे (घन्टाकर्ण)"), QStringLiteral("दुष्ट आत्माहरूलाई भगाउने नेवारी पर्व।"), QStringLiteral("श्रावण"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("चतुर्दशी"), EventRule::Udaya, false},
    {QStringLiteral("नाग पञ्चमी"), QStringLiteral("सर्प देवताको पूजा गर्ने र ढोकामा नाग टाँस्ने दिन।"), QStringLiteral("श्रावण"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पञ्चमी"), EventRule::Udaya, false},
    {QStringLiteral("जनै पूर्णिमा / रक्षा बन्धन"), QStringLiteral("जनै बदल्ने तथा रक्षा बन्धन बाँध्ने, क्वाँटी खाने दिन।"), QStringLiteral("श्रावण"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Udaya, true},
    {QStringLiteral("कृष्ण जन्माष्टमी"), QStringLiteral("भगवान कृष्णको जन्मदिन (निशीथ काल)।"), QStringLiteral("भाद्र"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अष्टमी"), EventRule::Nishitha, true},
    {QStringLiteral("गाईजात्रा"), QStringLiteral("दिवंगत आत्माको शान्तिको लागि मनाइने उत्सव।"), QStringLiteral("भाद्र"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, true},
    {QStringLiteral("कुशे औंसी (बुवाको मुख हेर्ने)"), QStringLiteral("बुवाको सम्मान गर्ने दिन।"), QStringLiteral("भाद्र"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अमावस्या"), EventRule::Udaya, false},
    {QStringLiteral("हरितालिका तीज"), QStringLiteral("महिलाहरूको महान उत्सव, शिव-पार्वतीको पूजा।"), QStringLiteral("भाद्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("तृतीया"), EventRule::Udaya, true},
    {QStringLiteral("गणेश चतुर्थी"), QStringLiteral("भगवान गणेशको जन्मदिन (मध्याह्न काल)।"), QStringLiteral("भाद्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("चतुर्थी"), EventRule::Madhyanna, false},
    {QStringLiteral("ऋषि पञ्चमी"), QStringLiteral("महिलाहरूले सप्तऋषिको पूजा गर्ने विशेष दिन।"), QStringLiteral("भाद्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पञ्चमी"), EventRule::Udaya, true},
    {QStringLiteral("इन्द्रजात्रा"), QStringLiteral("काठमाडौँको प्रमुख सडक उत्सव, जीवित देवी कुमारीको रथ यात्रा।"), QStringLiteral("भाद्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("चतुर्दशी"), EventRule::Udaya, true},
    {QStringLiteral("सोह्र श्राद्ध आरम्भ"), QStringLiteral("पितृ पक्ष (सोह्र श्राद्ध) सुरु हुने दिन।"), QStringLiteral("भाद्र"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Udaya, true},
    {QStringLiteral("प्रतिपदा श्राद्ध"), QStringLiteral("प्रतिपदा तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, false},
    {QStringLiteral("द्वितीया श्राद्ध"), QStringLiteral("द्वितीया तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("द्वितीया"), EventRule::Udaya, false},
    {QStringLiteral("तृतीया श्राद्ध"), QStringLiteral("तृतीया तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("तृतीया"), EventRule::Udaya, false},
    {QStringLiteral("चतुर्थी श्राद्ध"), QStringLiteral("चतुर्थी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("चतुर्थी"), EventRule::Udaya, false},
    {QStringLiteral("पञ्चमी श्राद्ध"), QStringLiteral("पञ्चमी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("पञ्चमी"), EventRule::Udaya, false},
    {QStringLiteral("षष्ठी श्राद्ध"), QStringLiteral("षष्ठी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("षष्ठी"), EventRule::Udaya, false},
    {QStringLiteral("सप्तमी श्राद्ध"), QStringLiteral("सप्तमी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("सप्तमी"), EventRule::Udaya, false},
    {QStringLiteral("अष्टमी श्राद्ध"), QStringLiteral("अष्टमी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अष्टमी"), EventRule::Udaya, false},
    {QStringLiteral("नवमी श्राद्ध"), QStringLiteral("नवमी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("नवमी"), EventRule::Udaya, false},
    {QStringLiteral("दशमी श्राद्ध"), QStringLiteral("दशमी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("दशमी"), EventRule::Udaya, false},
    {QStringLiteral("एकादशी श्राद्ध"), QStringLiteral("एकादशी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("एकादशी"), EventRule::Udaya, false},
    {QStringLiteral("द्वादशी श्राद्ध"), QStringLiteral("द्वादशी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("द्वादशी"), EventRule::Udaya, false},
    {QStringLiteral("त्रयोदशी श्राद्ध"), QStringLiteral("त्रयोदशी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("त्रयोदशी"), EventRule::Udaya, false},
    {QStringLiteral("चतुर्दशी श्राद्ध"), QStringLiteral("चतुर्दशी तिथिमा पर्ने पितृको श्राद्ध गर्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("चतुर्दशी"), EventRule::Udaya, false},
    {QStringLiteral("औंसी श्राद्ध (पितृ विसर्जन)"), QStringLiteral("पितृ पक्ष समाप्त हुने दिन, सबै पितृको सामूहिक श्राद्ध।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अमावस्या"), EventRule::Udaya, false},
    {QStringLiteral("जितिया पर्व"), QStringLiteral("तराईका महिलाहरूले सन्तानको दीर्घायुको लागि मनाउने व्रत।"), QStringLiteral("आश्विन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अष्टमी"), EventRule::Udaya, true},
    {QStringLiteral("घटस्थापना"), QStringLiteral("दशैँको सुरुवात, जमरा राख्ने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, true},
    {QStringLiteral("फूलपाती"), QStringLiteral("दशैँको सातौँ दिन।"), QStringLiteral("आश्विन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("सप्तमी"), EventRule::Madhyanna, true},
    {QStringLiteral("महाअष्टमी"), QStringLiteral("दशैँको आठौँ दिन, कालरात्रिको पूजा।"), QStringLiteral("आश्विन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("अष्टमी"), EventRule::Madhyanna, true},
    {QStringLiteral("महानवमी"), QStringLiteral("दशैँको नवौं दिन, विश्वकर्मा पूजा।"), QStringLiteral("आश्विन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("नवमी"), EventRule::Udaya, true},
    {QStringLiteral("विजयादशमी (दशैं)"), QStringLiteral("दशैँको मुख्य दिन, टिका र जमरा लगाउने दिन।"), QStringLiteral("आश्विन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("दशमी"), EventRule::Udaya, true},
    {QStringLiteral("कोजाग्रत पूर्णिमा"), QStringLiteral("दशैँको अन्त्य, धनकी देवी लक्ष्मीको पूजा।"), QStringLiteral("आश्विन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Nishitha, false},
    {QStringLiteral("काग तिहार"), QStringLiteral("तिहारको पहिलो दिन, कागलाई समर्पित।"), QStringLiteral("कार्तिक"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("त्रयोदशी"), EventRule::Udaya, true},
    {QStringLiteral("कुकुर तिहार"), QStringLiteral("तिहारको दोस्रो दिन, कुकुरलाई समर्पित।"), QStringLiteral("कार्तिक"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("चतुर्दशी"), EventRule::Udaya, true},
    {QStringLiteral("लक्ष्मी पूजा"), QStringLiteral("तिहारको तेस्रो दिन, गाई तिहार र लक्ष्मी पूजा (प्रदोष काल)।"), QStringLiteral("कार्तिक"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अमावस्या"), EventRule::Pradosh, true},
    {QStringLiteral("गोवर्धन पूजा / म्ह पूजा"), QStringLiteral("तिहारको चौथो दिन, गोवर्धन पर्वत और आत्म-पूजा (म्ह पूजा)।"), QStringLiteral("कार्तिक"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, true},
    {QStringLiteral("भाइटीका"), QStringLiteral("तिहारको पाँचौँ दिन, दाजुभाइ र दिदीबहिनीको पर्व।"), QStringLiteral("कार्तिक"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("द्वितीया"), EventRule::Madhyanna, true},
    {QStringLiteral("छठ पर्व"), QStringLiteral("सूर्य देवतालाई समर्पित तराईको महान पर्व (साँझको अर्घ्य)।"), QStringLiteral("कार्तिक"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("षष्ठी"), EventRule::Pradosh, true},
    {QStringLiteral("हरिबोधिनी एकादशी (तुलसी विवाह)"), QStringLiteral("भगवान विष्णुको जागरण, तुलसी विवाह गर्ने दिन।"), QStringLiteral("कार्तिक"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("एकादशी"), EventRule::Udaya, false},
    {QStringLiteral("बाला चतुर्दशी"), QStringLiteral("पशुपतिनाथमा शतबीज छर्ने विशेष दिन।"), QStringLiteral("मार्गशीर्ष"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("चतुर्दशी"), EventRule::Udaya, false},
    {QStringLiteral("विवाह पञ्चमी"), QStringLiteral("जनकपुरमा भगवान राम र सीताको विवाह महोत्सव।"), QStringLiteral("मार्गशीर्ष"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पञ्चमी"), EventRule::Udaya, false},
    {QStringLiteral("य:मरि पुन्हि / धान्यपुर्णिमा"), QStringLiteral("नेवार समुदायको य:मरि खाने र धान्य पूर्णिमाको उत्सव।"), QStringLiteral("मार्गशीर्ष"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Udaya, false},
    {QStringLiteral("पितृ स्मृती दिवस"), QStringLiteral("पितृ को विशेश सम्झना तथा श्रद्दाञ्जलि गर्ने दिन।"), QStringLiteral("पौष"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("अष्टमी"), EventRule::Udaya, false},
    {QStringLiteral("स्वस्थानी व्रत आरम्भ"), QStringLiteral("श्री स्वस्थानी माताको व्रत तथा कथा सुरु हुने दिन।"), QStringLiteral("पौष"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Udaya, false},
    {QStringLiteral("सोनाम ल्होछार"), QStringLiteral("तामाङ समुदायको नयाँ वर्ष।"), QStringLiteral("माघ"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, true},
    {QStringLiteral("सरस्वती पूजा / वसन्तपञ्चमी"), QStringLiteral("ज्ञान र शिक्षाको देवी सरस्वतीको पूजा, वसन्त ऋतुको आगमन।"), QStringLiteral("माघ"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पञ्चमी"), EventRule::Udaya, true},
    {QStringLiteral("ग्याल्पो ल्होछार"), QStringLiteral("शेर्पा समुदायको नयाँ वर्ष।"), QStringLiteral("फाल्गुन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Udaya, true},
    {QStringLiteral("महाशिवरात्रि"), QStringLiteral("भगवान शिवको महान रात्रि, पशुपतिनाथमा मेला (निशीथ काल)।"), QStringLiteral("फाल्गुन"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("चतुर्दशी"), EventRule::Nishitha, true},
    {QStringLiteral("होलिका दहन"), QStringLiteral("फागु पूर्णिमाको साँझमा होलिका दहन गरिने।"), QStringLiteral("फाल्गुन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Pradosh, false},
    {QStringLiteral("होली (पहाड)"), QStringLiteral("रङ्गहरूको उत्सव (पहाडी जिल्लाहरूमा)।"), QStringLiteral("फाल्गुन"), QStringLiteral("शुक्ल पक्ष"), QStringLiteral("पूर्णिमा"), EventRule::Pradosh, true},
    {QStringLiteral("होली (तराई)"), QStringLiteral("रङ्गहरूको उत्सव (तराई जिल्लाहरूमा)।"), QStringLiteral("चैत्र"), QStringLiteral("कृष्ण पक्ष"), QStringLiteral("प्रतिपदा"), EventRule::Pradosh, true},
};
const int numLunarEvents = sizeof(lunarEvents) / sizeof(lunarEvents[0]);

const FixedEventRule gregorianEvents[] = {
    {QStringLiteral("अङ्ग्रेजी नयाँ वर्ष"), QStringLiteral("ग्रेगोरियन क्यालेन्डरको पहिलो दिन।"), 1, 1, 0, true},
    {QStringLiteral("विश्व ब्रेल दिवस"), QStringLiteral("ब्रेल लिपिको महत्व झल्काउने दिन।"), 1, 4, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय शिक्षा दिवस"), QStringLiteral("शान्ति र विकासका लागि शिक्षाको भूमिका।"), 1, 24, 0, false},
    {QStringLiteral("विश्व क्यान्सर दिवस"), QStringLiteral("क्यान्सर विरुद्ध जनचेतना फैलाउने दिन।"), 2, 4, 0, false},
    {QStringLiteral("भ्यालेन्टाइन डे"), QStringLiteral("प्रेमको उत्सव मनाउने दिन।"), 2, 14, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय मातृभाषा दिवस"), QStringLiteral("सांस्कृतिक विविधता र मातृभाषाको संरक्षण।"), 2, 21, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय नारी दिवस"), QStringLiteral("महिला अधिकार र सशक्तिकरणको लागि।"), 3, 8, 0, true},
    {QStringLiteral("विश्व पानी दिवस"), QStringLiteral("स्वच्छ पानीको महत्व र संरक्षण।"), 3, 22, 0, false},
    {QStringLiteral("विश्व स्वास्थ्य दिवस"), QStringLiteral("ग्लोबल स्वास्थ्य जागरूकताका लागि।"), 4, 7, 0, false},
    {QStringLiteral("पृथ्वी दिवस"), QStringLiteral("पर्यावरण संरक्षणको लागि विश्वव्यापी दिन।"), 4, 22, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय श्रमिक दिवस / मदजुर दिवस"), QStringLiteral("श्रमिकहरूको योगदानको सम्मानमा।"), 5, 1, 0, true},
    {QStringLiteral("विश्व प्रेस स्वतन्त्रता दिवस"), QStringLiteral("प्रेस स्वतन्त्रता र निर्भिक पत्रकारिताको लागि।"), 5, 3, 0, false},
    {QStringLiteral("विश्व रेडक्रस दिवस"), QStringLiteral("रेडक्रसको मानवीय सेवाको सम्मानमा।"), 5, 8, 0, false},
    {QStringLiteral("विश्व वातावरण दिवस"), QStringLiteral("पर्यावरण संरक्षणको जनचेतनाका लागि।"), 6, 5, 0, false},
    {QStringLiteral("विश्व शरणार्थी दिवस"), QStringLiteral("शरणार्थीहरूको साहस र संघर्षको सम्मानमा।"), 6, 20, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय योग दिवस"), QStringLiteral("योग र स्वस्थ जीवनशैलीको महत्व।"), 6, 21, 0, false},
    {QStringLiteral("विश्व जनसंख्या दिवस"), QStringLiteral("जनसंख्या सम्बन्धी चुनौतीहरूको बारेमा।"), 7, 11, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय युवा दिवस"), QStringLiteral("युवाहरूको सशक्तिकरण र सहभागिताको लागि।"), 8, 12, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय साक्षरता दिवस"), QStringLiteral("साक्षरताको महत्व झल्काउने दिन।"), 9, 8, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय लोकतन्त्र दिवस"), QStringLiteral("लोकतान्त्रिक मूल्य र मान्यताको प्रवर्द्धन।"), 9, 15, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय शान्ति दिवस"), QStringLiteral("विश्वभर शान्ति र अहिंसाको सन्देश।"), 9, 21, 0, false},
    {QStringLiteral("विश्व शिक्षक दिवस"), QStringLiteral("शिक्षकहरूको योगदानको कदर गर्ने दिन।"), 10, 5, 0, false},
    {QStringLiteral("विश्व खाद्य दिवस"), QStringLiteral("खाद्य सुरक्षा र भोकमरी विरुद्धको चेतना।"), 10, 16, 0, false},
    {QStringLiteral("संयुक्त राष्ट्र संघ दिवस"), QStringLiteral("संयुक्त राष्ट्र संघको स्थापना दिवस।"), 10, 24, 0, false},
    {QStringLiteral("विश्व बाल दिवस"), QStringLiteral("बाल अधिकार र कल्याणको प्रवर्द्धन।"), 11, 20, 0, false},
    {QStringLiteral("विश्व एड्स दिवस"), QStringLiteral("एड्स विरुद्ध जनचेतना फैलाउने दिन।"), 12, 1, 0, false},
    {QStringLiteral("अन्तर्राष्ट्रिय मानव अधिकार दिवस"), QStringLiteral("मानव अधिकारको विश्वव्यापी घोषणापत्र जारी भएको दिन।"), 12, 10, 0, false},
    {QStringLiteral("क्रिसमस डे"), QStringLiteral("येशु ख्रिष्टको जन्मोत्सव।"), 12, 25, 0, true},
    {QStringLiteral("अङ्ग्रेजी नयाँ वर्षको अघिल्लो दिन"), QStringLiteral("ग्रेगोरियन वर्षको अन्तिम दिन।"), 12, 31, 0, true},
};
const int numGregorianEvents = sizeof(gregorianEvents) / sizeof(gregorianEvents[0]);

const FixedEventRule bikramEvents[] = {
    {QStringLiteral("लोकतन्त्र दिवस"), QStringLiteral("लोकतन्त्रको बहाली भएको दिन।"), 1, 11, 0, true},
    {QStringLiteral("गणतन्त्र दिवस"), QStringLiteral("नेपाल संघीय लोकतान्त्रिक गणतन्त्र घोषणा भएको दिन।"), 2, 15, 0, true},
    {QStringLiteral("राष्ट्रिय धान दिवस"), QStringLiteral("असार १५, हिलो खेल्ने र दही चिउरा खाने दिन।"), 3, 15, 0, false},
    {QStringLiteral("भानु जयन्ती"), QStringLiteral("आदिकवि भानुभक्त आचार्यको जन्म जयन्ती।"), 3, 29, 0, false},
    {QStringLiteral("संविधान दिवस"), QStringLiteral("नयाँ संविधान जारी भएको ऐतिहासिक दिन।"), 6, 3, 0, true},
    {QStringLiteral("पृथ्वी जयन्ती / राष्ट्रिय एकता दिवस"), QStringLiteral("नेपालको एकीकरण गर्ने राजा पृथ्वीनारायण शाहको जन्मदिन।"), 9, 27, 0, true},
    {QStringLiteral("शहीद दिवस"), QStringLiteral("वीर शहीदहरूको सम्झना और सम्मानमा।"), 10, 16, 0, true},
    {QStringLiteral("प्रजातन्त्र दिवस"), QStringLiteral("२००७ सालको क्रान्ति सफल भई प्रजातन्त्र आएको दिन।"), 11, 7, 0, true},
    {QStringLiteral("बिस्का: जात्रा"), QStringLiteral("भक्तपुरमा नयाँ वर्षको अवसरमा मनाइने ऐतिहासिक जात्रा।"), 1, 1, 0, true},
    {QStringLiteral("बैशाख सङ्क्रान्ति"), QStringLiteral("नयाँ वर्षको प्रारम्भ, सूर्य मेष राशिमा प्रवेश।"), 1, 1, 0, true},
    {QStringLiteral("जेठ सङ्क्रान्ति"), QStringLiteral("सूर्य वृष राशिमा प्रवेश।"), 2, 1, 0, false},
    {QStringLiteral("असार सङ्क्रान्ति"), QStringLiteral("सूर्य मिथुन राशिमा प्रवेश।"), 3, 1, 0, false},
    {QStringLiteral("साउने सङ्क्रान्ति"), QStringLiteral("सूर्य कर्कट राशिमा प्रवेश।"), 4, 1, 0, false},
    {QStringLiteral("भदौ सङ्क्रान्ति"), QStringLiteral("सूर्य सिंह राशिमा प्रवेश।"), 5, 1, 0, false},
    {QStringLiteral("असोज सङ्क्रान्ति"), QStringLiteral("सूर्य कन्या राशिमा प्रवेश।"), 6, 1, 0, false},
    {QStringLiteral("कार्तिक सङ्क्रान्ति"), QStringLiteral("सूर्य तुला राशिमा प्रवेश।"), 7, 1, 0, false},
    {QStringLiteral("मङ्सिर सङ्क्रान्ति"), QStringLiteral("सूर्य वृश्चिक राशिमा प्रवेश।"), 8, 1, 0, false},
    {QStringLiteral("पुष सङ्क्रान्ति"), QStringLiteral("सूर्य धनु राशिमा प्रवेश।"), 9, 1, 0, false},
    {QStringLiteral("माघे सङ्क्रान्ति / उत्तरायण"), QStringLiteral("मकर संक्रान्ति र उत्तरायणको आरम्भ।"), 10, 1, 0, true},
    {QStringLiteral("फागुन सङ्क्रान्ति"), QStringLiteral("सूर्य कुम्भ राशिमा प्रवेश।"), 11, 1, 0, false},
    {QStringLiteral("चैत सङ्क्रान्ति"), QStringLiteral("सूर्य मीन राशिमा प्रवेश।"), 12, 1, 0, false},
};
const int numBikramEvents = sizeof(bikramEvents) / sizeof(bikramEvents[0]);
