<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.4">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="xilinx_devices">
<packages>
<package name="VQ100">
<description>&lt;b&gt;VQ100&lt;/b&gt;</description>
<wire x1="-6.625" y1="6.275" x2="-6.625" y2="-6.275" width="0.127" layer="51"/>
<wire x1="-6.625" y1="-6.275" x2="-6.275" y2="-6.625" width="0.127" layer="51"/>
<wire x1="-6.275" y1="-6.625" x2="6.275" y2="-6.625" width="0.127" layer="51"/>
<wire x1="6.275" y1="-6.625" x2="6.625" y2="-6.275" width="0.127" layer="51"/>
<wire x1="6.625" y1="-6.275" x2="6.625" y2="6.275" width="0.127" layer="51"/>
<wire x1="6.625" y1="6.275" x2="6.275" y2="6.625" width="0.127" layer="51"/>
<wire x1="6.275" y1="6.625" x2="-6.275" y2="6.625" width="0.127" layer="51"/>
<wire x1="-6.275" y1="6.625" x2="-6.625" y2="6.275" width="0.127" layer="51"/>
<wire x1="-7" y1="6.5" x2="-7" y2="-6.5" width="0.254" layer="21"/>
<wire x1="-7" y1="-6.5" x2="-6.5" y2="-7" width="0.254" layer="21"/>
<wire x1="-6.5" y1="-7" x2="6.5" y2="-7" width="0.254" layer="21"/>
<wire x1="6.5" y1="-7" x2="7" y2="-6.5" width="0.254" layer="21"/>
<wire x1="7" y1="-6.5" x2="7" y2="6.5" width="0.254" layer="21"/>
<wire x1="7" y1="6.5" x2="6.5" y2="7" width="0.254" layer="21"/>
<wire x1="6.5" y1="7" x2="-6.5" y2="7" width="0.254" layer="21"/>
<wire x1="-6.5" y1="7" x2="-7" y2="6.5" width="0.254" layer="21"/>
<circle x="-5" y="5" radius="0.5" width="0.127" layer="21"/>
<smd name="P1" x="-8.1" y="6" dx="1.6" dy="0.3" layer="1"/>
<smd name="P2" x="-8.1" y="5.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P3" x="-8.1" y="5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P4" x="-8.1" y="4.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P5" x="-8.1" y="4" dx="1.6" dy="0.3" layer="1"/>
<smd name="P6" x="-8.1" y="3.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P7" x="-8.1" y="3" dx="1.6" dy="0.3" layer="1"/>
<smd name="P8" x="-8.1" y="2.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P9" x="-8.1" y="2" dx="1.6" dy="0.3" layer="1"/>
<smd name="P10" x="-8.1" y="1.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P11" x="-8.1" y="1" dx="1.6" dy="0.3" layer="1"/>
<smd name="P12" x="-8.1" y="0.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P13" x="-8.1" y="0" dx="1.6" dy="0.3" layer="1"/>
<smd name="P14" x="-8.1" y="-0.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P15" x="-8.1" y="-1" dx="1.6" dy="0.3" layer="1"/>
<smd name="P16" x="-8.1" y="-1.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P17" x="-8.1" y="-2" dx="1.6" dy="0.3" layer="1"/>
<smd name="P18" x="-8.1" y="-2.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P19" x="-8.1" y="-3" dx="1.6" dy="0.3" layer="1"/>
<smd name="P20" x="-8.1" y="-3.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P21" x="-8.1" y="-4" dx="1.6" dy="0.3" layer="1"/>
<smd name="P22" x="-8.1" y="-4.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P23" x="-8.1" y="-5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P24" x="-8.1" y="-5.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P25" x="-8.1" y="-6" dx="1.6" dy="0.3" layer="1"/>
<smd name="P26" x="-6" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P27" x="-5.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P28" x="-5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P29" x="-4.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P30" x="-4" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P31" x="-3.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P32" x="-3" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P33" x="-2.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P34" x="-2" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P35" x="-1.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P36" x="-1" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P37" x="-0.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P38" x="0" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P39" x="0.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P40" x="1" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P41" x="1.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P42" x="2" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P43" x="2.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P44" x="3" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P45" x="3.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P46" x="4" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P47" x="4.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P48" x="5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P49" x="5.5" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P50" x="6" y="-8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P51" x="8.1" y="-6" dx="1.6" dy="0.3" layer="1"/>
<smd name="P52" x="8.1" y="-5.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P53" x="8.1" y="-5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P54" x="8.1" y="-4.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P55" x="8.1" y="-4" dx="1.6" dy="0.3" layer="1"/>
<smd name="P56" x="8.1" y="-3.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P57" x="8.1" y="-3" dx="1.6" dy="0.3" layer="1"/>
<smd name="P58" x="8.1" y="-2.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P59" x="8.1" y="-2" dx="1.6" dy="0.3" layer="1"/>
<smd name="P60" x="8.1" y="-1.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P61" x="8.1" y="-1" dx="1.6" dy="0.3" layer="1"/>
<smd name="P62" x="8.1" y="-0.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P63" x="8.1" y="0" dx="1.6" dy="0.3" layer="1"/>
<smd name="P64" x="8.1" y="0.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P65" x="8.1" y="1" dx="1.6" dy="0.3" layer="1"/>
<smd name="P66" x="8.1" y="1.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P67" x="8.1" y="2" dx="1.6" dy="0.3" layer="1"/>
<smd name="P68" x="8.1" y="2.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P69" x="8.1" y="3" dx="1.6" dy="0.3" layer="1"/>
<smd name="P70" x="8.1" y="3.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P71" x="8.1" y="4" dx="1.6" dy="0.3" layer="1"/>
<smd name="P72" x="8.1" y="4.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P73" x="8.1" y="5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P74" x="8.1" y="5.5" dx="1.6" dy="0.3" layer="1"/>
<smd name="P75" x="8.1" y="6" dx="1.6" dy="0.3" layer="1"/>
<smd name="P76" x="6" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P77" x="5.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P78" x="5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P79" x="4.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P80" x="4" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P81" x="3.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P82" x="3" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P83" x="2.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P84" x="2" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P85" x="1.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P86" x="1" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P87" x="0.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P88" x="0" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P89" x="-0.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P90" x="-1" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P91" x="-1.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P92" x="-2" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P93" x="-2.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P94" x="-3" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P95" x="-3.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P96" x="-4" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P97" x="-4.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P98" x="-5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P99" x="-5.5" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<smd name="P100" x="-6" y="8.1" dx="1.6" dy="0.3" layer="1" rot="R90"/>
<text x="-2" y="1" size="0.8" layer="25" font="vector" ratio="19">&gt;NAME</text>
<text x="-2.5" y="-2" size="0.8" layer="27" font="vector" ratio="19">&gt;VALUE</text>
<rectangle x1="-8" y1="5.87" x2="-7" y2="6.13" layer="51"/>
<rectangle x1="-8" y1="5.37" x2="-7" y2="5.63" layer="51"/>
<rectangle x1="-8" y1="4.87" x2="-7" y2="5.13" layer="51"/>
<rectangle x1="-8" y1="4.37" x2="-7" y2="4.63" layer="51"/>
<rectangle x1="-8" y1="3.87" x2="-7" y2="4.13" layer="51"/>
<rectangle x1="-8" y1="3.37" x2="-7" y2="3.63" layer="51"/>
<rectangle x1="-8" y1="2.87" x2="-7" y2="3.13" layer="51"/>
<rectangle x1="-8" y1="2.37" x2="-7" y2="2.63" layer="51"/>
<rectangle x1="-8" y1="1.87" x2="-7" y2="2.13" layer="51"/>
<rectangle x1="-8" y1="1.37" x2="-7" y2="1.63" layer="51"/>
<rectangle x1="-8" y1="0.87" x2="-7" y2="1.13" layer="51"/>
<rectangle x1="-8" y1="0.37" x2="-7" y2="0.63" layer="51"/>
<rectangle x1="-8" y1="-0.13" x2="-7" y2="0.13" layer="51"/>
<rectangle x1="-8" y1="-0.63" x2="-7" y2="-0.37" layer="51"/>
<rectangle x1="-8" y1="-1.13" x2="-7" y2="-0.87" layer="51"/>
<rectangle x1="-8" y1="-1.63" x2="-7" y2="-1.37" layer="51"/>
<rectangle x1="-8" y1="-2.13" x2="-7" y2="-1.87" layer="51"/>
<rectangle x1="-8" y1="-2.63" x2="-7" y2="-2.37" layer="51"/>
<rectangle x1="-8" y1="-3.13" x2="-7" y2="-2.87" layer="51"/>
<rectangle x1="-8" y1="-3.63" x2="-7" y2="-3.37" layer="51"/>
<rectangle x1="-8" y1="-4.13" x2="-7" y2="-3.87" layer="51"/>
<rectangle x1="-8" y1="-4.63" x2="-7" y2="-4.37" layer="51"/>
<rectangle x1="-8" y1="-5.13" x2="-7" y2="-4.87" layer="51"/>
<rectangle x1="-8" y1="-5.63" x2="-7" y2="-5.37" layer="51"/>
<rectangle x1="-8" y1="-6.13" x2="-7" y2="-5.87" layer="51"/>
<rectangle x1="-6.13" y1="-8" x2="-5.87" y2="-7" layer="51"/>
<rectangle x1="-5.63" y1="-8" x2="-5.37" y2="-7" layer="51"/>
<rectangle x1="-5.13" y1="-8" x2="-4.87" y2="-7" layer="51"/>
<rectangle x1="-4.63" y1="-8" x2="-4.37" y2="-7" layer="51"/>
<rectangle x1="-4.13" y1="-8" x2="-3.87" y2="-7" layer="51"/>
<rectangle x1="-3.63" y1="-8" x2="-3.37" y2="-7" layer="51"/>
<rectangle x1="-3.13" y1="-8" x2="-2.87" y2="-7" layer="51"/>
<rectangle x1="-2.63" y1="-8" x2="-2.37" y2="-7" layer="51"/>
<rectangle x1="-2.13" y1="-8" x2="-1.87" y2="-7" layer="51"/>
<rectangle x1="-1.63" y1="-8" x2="-1.37" y2="-7" layer="51"/>
<rectangle x1="-1.13" y1="-8" x2="-0.87" y2="-7" layer="51"/>
<rectangle x1="-0.63" y1="-8" x2="-0.37" y2="-7" layer="51"/>
<rectangle x1="-0.13" y1="-8" x2="0.13" y2="-7" layer="51"/>
<rectangle x1="0.37" y1="-8" x2="0.63" y2="-7" layer="51"/>
<rectangle x1="0.87" y1="-8" x2="1.13" y2="-7" layer="51"/>
<rectangle x1="1.37" y1="-8" x2="1.63" y2="-7" layer="51"/>
<rectangle x1="1.87" y1="-8" x2="2.13" y2="-7" layer="51"/>
<rectangle x1="2.37" y1="-8" x2="2.63" y2="-7" layer="51"/>
<rectangle x1="2.87" y1="-8" x2="3.13" y2="-7" layer="51"/>
<rectangle x1="3.37" y1="-8" x2="3.63" y2="-7" layer="51"/>
<rectangle x1="3.87" y1="-8" x2="4.13" y2="-7" layer="51"/>
<rectangle x1="4.37" y1="-8" x2="4.63" y2="-7" layer="51"/>
<rectangle x1="4.87" y1="-8" x2="5.13" y2="-7" layer="51"/>
<rectangle x1="5.37" y1="-8" x2="5.63" y2="-7" layer="51"/>
<rectangle x1="5.87" y1="-8" x2="6.13" y2="-7" layer="51"/>
<rectangle x1="7" y1="-6.13" x2="8" y2="-5.87" layer="51"/>
<rectangle x1="7" y1="-5.63" x2="8" y2="-5.37" layer="51"/>
<rectangle x1="7" y1="-5.13" x2="8" y2="-4.87" layer="51"/>
<rectangle x1="7" y1="-4.63" x2="8" y2="-4.37" layer="51"/>
<rectangle x1="7" y1="-4.13" x2="8" y2="-3.87" layer="51"/>
<rectangle x1="7" y1="-3.63" x2="8" y2="-3.37" layer="51"/>
<rectangle x1="7" y1="-3.13" x2="8" y2="-2.87" layer="51"/>
<rectangle x1="7" y1="-2.63" x2="8" y2="-2.37" layer="51"/>
<rectangle x1="7" y1="-2.13" x2="8" y2="-1.87" layer="51"/>
<rectangle x1="7" y1="-1.63" x2="8" y2="-1.37" layer="51"/>
<rectangle x1="7" y1="-1.13" x2="8" y2="-0.87" layer="51"/>
<rectangle x1="7" y1="-0.63" x2="8" y2="-0.37" layer="51"/>
<rectangle x1="7" y1="-0.13" x2="8" y2="0.13" layer="51"/>
<rectangle x1="7" y1="0.37" x2="8" y2="0.63" layer="51"/>
<rectangle x1="7" y1="0.87" x2="8" y2="1.13" layer="51"/>
<rectangle x1="7" y1="1.37" x2="8" y2="1.63" layer="51"/>
<rectangle x1="7" y1="1.87" x2="8" y2="2.13" layer="51"/>
<rectangle x1="7" y1="2.37" x2="8" y2="2.63" layer="51"/>
<rectangle x1="7" y1="2.87" x2="8" y2="3.13" layer="51"/>
<rectangle x1="7" y1="3.37" x2="8" y2="3.63" layer="51"/>
<rectangle x1="7" y1="3.87" x2="8" y2="4.13" layer="51"/>
<rectangle x1="7" y1="4.37" x2="8" y2="4.63" layer="51"/>
<rectangle x1="7" y1="4.87" x2="8" y2="5.13" layer="51"/>
<rectangle x1="7" y1="5.37" x2="8" y2="5.63" layer="51"/>
<rectangle x1="7" y1="5.87" x2="8" y2="6.13" layer="51"/>
<rectangle x1="5.87" y1="7" x2="6.13" y2="8" layer="51"/>
<rectangle x1="5.37" y1="7" x2="5.63" y2="8" layer="51"/>
<rectangle x1="4.87" y1="7" x2="5.13" y2="8" layer="51"/>
<rectangle x1="4.37" y1="7" x2="4.63" y2="8" layer="51"/>
<rectangle x1="3.87" y1="7" x2="4.13" y2="8" layer="51"/>
<rectangle x1="3.37" y1="7" x2="3.63" y2="8" layer="51"/>
<rectangle x1="2.87" y1="7" x2="3.13" y2="8" layer="51"/>
<rectangle x1="2.37" y1="7" x2="2.63" y2="8" layer="51"/>
<rectangle x1="1.87" y1="7" x2="2.13" y2="8" layer="51"/>
<rectangle x1="1.37" y1="7" x2="1.63" y2="8" layer="51"/>
<rectangle x1="0.87" y1="7" x2="1.13" y2="8" layer="51"/>
<rectangle x1="0.37" y1="7" x2="0.63" y2="8" layer="51"/>
<rectangle x1="-0.13" y1="7" x2="0.13" y2="8" layer="51"/>
<rectangle x1="-0.63" y1="7" x2="-0.37" y2="8" layer="51"/>
<rectangle x1="-1.13" y1="7" x2="-0.87" y2="8" layer="51"/>
<rectangle x1="-1.63" y1="7" x2="-1.37" y2="8" layer="51"/>
<rectangle x1="-2.13" y1="7" x2="-1.87" y2="8" layer="51"/>
<rectangle x1="-2.63" y1="7" x2="-2.37" y2="8" layer="51"/>
<rectangle x1="-3.13" y1="7" x2="-2.87" y2="8" layer="51"/>
<rectangle x1="-3.63" y1="7" x2="-3.37" y2="8" layer="51"/>
<rectangle x1="-4.13" y1="7" x2="-3.87" y2="8" layer="51"/>
<rectangle x1="-4.63" y1="7" x2="-4.37" y2="8" layer="51"/>
<rectangle x1="-5.13" y1="7" x2="-4.87" y2="8" layer="51"/>
<rectangle x1="-5.63" y1="7" x2="-5.37" y2="8" layer="51"/>
<rectangle x1="-6.13" y1="7" x2="-5.87" y2="8" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="XC3S50AVQ100_0">
<wire x1="-10.16" y1="22.86" x2="0" y2="22.86" width="0.254" layer="94"/>
<wire x1="0" y1="22.86" x2="0" y2="-22.86" width="0.254" layer="94"/>
<wire x1="0" y1="-22.86" x2="-10.16" y2="-22.86" width="0.254" layer="94"/>
<text x="0" y="23.622" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-25.4" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="VCCO_0@0" x="5.08" y="20.32" length="middle" direction="in" rot="R180"/>
<pin name="VCCO_0@1" x="5.08" y="17.78" length="middle" direction="in" rot="R180"/>
<pin name="IO_0/GCLK11" x="5.08" y="15.24" length="middle" rot="R180"/>
<pin name="IO_L01P_0/VREF_0" x="5.08" y="12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L01N_0" x="5.08" y="10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02P_0/GCLK4" x="5.08" y="7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02N_0/GCLK5" x="5.08" y="5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03P_0/GCLK6" x="5.08" y="2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03N_0/GCLK7" x="5.08" y="0" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04P_0/GCLK8" x="5.08" y="-2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04N_0/GCLK9" x="5.08" y="-5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05P_0" x="5.08" y="-7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05N_0" x="5.08" y="-10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06P_0/VREF_0" x="5.08" y="-12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06N_0/PUDC_B" x="5.08" y="-15.24" length="middle" swaplevel="1" rot="R180"/>
<pin name="IP_0" x="5.08" y="-17.78" length="middle" direction="in" swaplevel="1" rot="R180"/>
<pin name="IP_0/VREF_0" x="5.08" y="-20.32" length="middle" direction="in" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="XC3S50AVQ100_1">
<wire x1="-10.16" y1="17.78" x2="0" y2="17.78" width="0.254" layer="94"/>
<wire x1="0" y1="17.78" x2="0" y2="-20.32" width="0.254" layer="94"/>
<wire x1="0" y1="-20.32" x2="-10.16" y2="-20.32" width="0.254" layer="94"/>
<text x="0" y="18.542" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-22.86" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="VCCO_1" x="5.08" y="15.24" length="middle" direction="in" rot="R180"/>
<pin name="IO_L01P_1" x="5.08" y="12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L01N_1" x="5.08" y="10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02P_1/RHCLK0" x="5.08" y="7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02N_1/RHCLK1" x="5.08" y="5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03P_1/RHCLK2" x="5.08" y="2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03N_1/TRDY1/RHCLK3" x="5.08" y="0" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04P_1/IRDY1/RHCLK6" x="5.08" y="-2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04N_1/RHCLK7" x="5.08" y="-5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05P_1" x="5.08" y="-7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05N_1" x="5.08" y="-10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06P_1" x="5.08" y="-12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06N_1" x="5.08" y="-15.24" length="middle" swaplevel="1" rot="R180"/>
<pin name="IP_1/VREF_1" x="5.08" y="-17.78" length="middle" direction="in" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="XC3S50AVQ100_2">
<wire x1="-10.16" y1="35.56" x2="0" y2="35.56" width="0.254" layer="94"/>
<wire x1="0" y1="35.56" x2="0" y2="-38.1" width="0.254" layer="94"/>
<wire x1="0" y1="-38.1" x2="-10.16" y2="-38.1" width="0.254" layer="94"/>
<text x="0" y="36.322" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-40.64" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="VCCO_2@0" x="5.08" y="33.02" length="middle" direction="in" rot="R180"/>
<pin name="VCCO_2@1" x="5.08" y="30.48" length="middle" direction="in" rot="R180"/>
<pin name="IO_2/MOSI/CSI_B" x="5.08" y="27.94" length="middle" rot="R180"/>
<pin name="IO_L01P_2/M1" x="5.08" y="25.4" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L01N_2/M0" x="5.08" y="22.86" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02P_2/M2" x="5.08" y="20.32" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02N_2/CSO_B" x="5.08" y="17.78" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03P_2/RDWR_B" x="5.08" y="15.24" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03N_2/VS1" x="5.08" y="12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04P_2/VS2" x="5.08" y="10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04N_2/VS0" x="5.08" y="7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05P_2" x="5.08" y="5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05N_2/D7" x="5.08" y="2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06P_2" x="5.08" y="0" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06N_2/D6" x="5.08" y="-2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L07P_2/D5" x="5.08" y="-5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L07N_2/D4" x="5.08" y="-7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L08P_2/GCLK14" x="5.08" y="-10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L08N_2/GCLK15" x="5.08" y="-12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L09P_2/GCLK0" x="5.08" y="-15.24" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L09N_2/GCLK1" x="5.08" y="-17.78" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L10P_2/INIT_B" x="5.08" y="-20.32" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L10N_2/D3" x="5.08" y="-22.86" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L11P_2/D2" x="5.08" y="-25.4" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L11N_2/D0/DIN/MISO" x="5.08" y="-27.94" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L12P_2/D1" x="5.08" y="-30.48" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L12N_2/CCLK" x="5.08" y="-33.02" length="middle" swaplevel="1" rot="R180"/>
<pin name="IP_2/VREF_2" x="5.08" y="-35.56" length="middle" direction="in" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="XC3S50AVQ100_3">
<wire x1="-10.16" y1="20.32" x2="0" y2="20.32" width="0.254" layer="94"/>
<wire x1="0" y1="20.32" x2="0" y2="-20.32" width="0.254" layer="94"/>
<wire x1="0" y1="-20.32" x2="-10.16" y2="-20.32" width="0.254" layer="94"/>
<text x="0" y="21.082" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-22.86" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="VCCO_3" x="5.08" y="17.78" length="middle" direction="in" rot="R180"/>
<pin name="IO_L01P_3" x="5.08" y="15.24" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L01N_3" x="5.08" y="12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02P_3" x="5.08" y="10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L02N_3" x="5.08" y="7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03P_3/LHCLK0" x="5.08" y="5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L03N_3/LHCLK1" x="5.08" y="2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04P_3/LHCLK2" x="5.08" y="0" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L04N_3/IRDY2/LHCLK3" x="5.08" y="-2.54" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05P_3/TRDY2/LHCLK6" x="5.08" y="-5.08" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L05N_3/LHCLK7" x="5.08" y="-7.62" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06P_3" x="5.08" y="-10.16" length="middle" swaplevel="1" rot="R180"/>
<pin name="IO_L06N_3" x="5.08" y="-12.7" length="middle" swaplevel="1" rot="R180"/>
<pin name="IP_3" x="5.08" y="-15.24" length="middle" direction="in" swaplevel="1" rot="R180"/>
<pin name="IP_3/VREF_3" x="5.08" y="-17.78" length="middle" direction="in" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="XC3S50AVQ100_GND">
<wire x1="-10.16" y1="17.78" x2="0" y2="17.78" width="0.254" layer="94"/>
<wire x1="0" y1="17.78" x2="0" y2="-17.78" width="0.254" layer="94"/>
<wire x1="0" y1="-17.78" x2="-10.16" y2="-17.78" width="0.254" layer="94"/>
<text x="0" y="18.542" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-20.32" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="GND@0" x="5.08" y="15.24" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@1" x="5.08" y="12.7" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@2" x="5.08" y="10.16" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@3" x="5.08" y="7.62" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@4" x="5.08" y="5.08" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@5" x="5.08" y="2.54" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@6" x="5.08" y="0" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@7" x="5.08" y="-2.54" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@8" x="5.08" y="-5.08" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@9" x="5.08" y="-7.62" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@10" x="5.08" y="-10.16" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@11" x="5.08" y="-12.7" length="middle" direction="pwr" rot="R180"/>
<pin name="GND@12" x="5.08" y="-15.24" length="middle" direction="pwr" rot="R180"/>
</symbol>
<symbol name="XC3S50AVQ100_VCCAUX">
<wire x1="-10.16" y1="12.7" x2="0" y2="12.7" width="0.254" layer="94"/>
<wire x1="0" y1="12.7" x2="0" y2="-12.7" width="0.254" layer="94"/>
<wire x1="0" y1="-12.7" x2="-10.16" y2="-12.7" width="0.254" layer="94"/>
<text x="0" y="13.462" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-15.24" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="VCCAUX@0" x="5.08" y="10.16" length="middle" direction="in" rot="R180"/>
<pin name="VCCAUX@1" x="5.08" y="7.62" length="middle" direction="in" rot="R180"/>
<pin name="VCCAUX@2" x="5.08" y="5.08" length="middle" direction="in" rot="R180"/>
<pin name="DONE" x="5.08" y="2.54" length="middle" rot="R180"/>
<pin name="PROG_B" x="5.08" y="0" length="middle" direction="in" rot="R180"/>
<pin name="TCK" x="5.08" y="-2.54" length="middle" direction="in" rot="R180"/>
<pin name="TDI" x="5.08" y="-5.08" length="middle" direction="in" rot="R180"/>
<pin name="TDO" x="5.08" y="-7.62" length="middle" direction="out" rot="R180"/>
<pin name="TMS" x="5.08" y="-10.16" length="middle" direction="in" rot="R180"/>
</symbol>
<symbol name="XC3S50AVQ100_VCCINT">
<wire x1="-10.16" y1="5.08" x2="0" y2="5.08" width="0.254" layer="94"/>
<wire x1="0" y1="5.08" x2="0" y2="-7.62" width="0.254" layer="94"/>
<wire x1="0" y1="-7.62" x2="-10.16" y2="-7.62" width="0.254" layer="94"/>
<text x="0" y="5.842" size="1.778" layer="95" rot="MR0">&gt;NAME</text>
<text x="0" y="-10.16" size="1.778" layer="96" rot="MR0">&gt;VALUE</text>
<pin name="VCCINT@0" x="5.08" y="2.54" length="middle" direction="in" rot="R180"/>
<pin name="VCCINT@1" x="5.08" y="0" length="middle" direction="in" rot="R180"/>
<pin name="VCCINT@2" x="5.08" y="-2.54" length="middle" direction="in" rot="R180"/>
<pin name="VCCINT@3" x="5.08" y="-5.08" length="middle" direction="in" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="XC3S50AVQ100" prefix="U">
<description>Xilinx FPGA: XC3S50AVQ100</description>
<gates>
<gate name="B0" symbol="XC3S50AVQ100_0" x="0" y="0"/>
<gate name="B1" symbol="XC3S50AVQ100_1" x="45.72" y="0"/>
<gate name="B2" symbol="XC3S50AVQ100_2" x="91.44" y="0"/>
<gate name="B3" symbol="XC3S50AVQ100_3" x="137.16" y="0"/>
<gate name="BGND" symbol="XC3S50AVQ100_GND" x="182.88" y="0"/>
<gate name="BVCCAUX" symbol="XC3S50AVQ100_VCCAUX" x="228.6" y="0"/>
<gate name="BVCCINT" symbol="XC3S50AVQ100_VCCINT" x="274.32" y="0"/>
</gates>
<devices>
<device name="" package="VQ100">
<connects>
<connect gate="B0" pin="IO_0/GCLK11" pad="P90"/>
<connect gate="B0" pin="IO_L01N_0" pad="P78"/>
<connect gate="B0" pin="IO_L01P_0/VREF_0" pad="P77"/>
<connect gate="B0" pin="IO_L02N_0/GCLK5" pad="P84"/>
<connect gate="B0" pin="IO_L02P_0/GCLK4" pad="P83"/>
<connect gate="B0" pin="IO_L03N_0/GCLK7" pad="P86"/>
<connect gate="B0" pin="IO_L03P_0/GCLK6" pad="P85"/>
<connect gate="B0" pin="IO_L04N_0/GCLK9" pad="P89"/>
<connect gate="B0" pin="IO_L04P_0/GCLK8" pad="P88"/>
<connect gate="B0" pin="IO_L05N_0" pad="P94"/>
<connect gate="B0" pin="IO_L05P_0" pad="P93"/>
<connect gate="B0" pin="IO_L06N_0/PUDC_B" pad="P99"/>
<connect gate="B0" pin="IO_L06P_0/VREF_0" pad="P98"/>
<connect gate="B0" pin="IP_0" pad="P97"/>
<connect gate="B0" pin="IP_0/VREF_0" pad="P82"/>
<connect gate="B0" pin="VCCO_0@0" pad="P79"/>
<connect gate="B0" pin="VCCO_0@1" pad="P96"/>
<connect gate="B1" pin="IO_L01N_1" pad="P57"/>
<connect gate="B1" pin="IO_L01P_1" pad="P56"/>
<connect gate="B1" pin="IO_L02N_1/RHCLK1" pad="P60"/>
<connect gate="B1" pin="IO_L02P_1/RHCLK0" pad="P59"/>
<connect gate="B1" pin="IO_L03N_1/TRDY1/RHCLK3" pad="P62"/>
<connect gate="B1" pin="IO_L03P_1/RHCLK2" pad="P61"/>
<connect gate="B1" pin="IO_L04N_1/RHCLK7" pad="P65"/>
<connect gate="B1" pin="IO_L04P_1/IRDY1/RHCLK6" pad="P64"/>
<connect gate="B1" pin="IO_L05N_1" pad="P71"/>
<connect gate="B1" pin="IO_L05P_1" pad="P70"/>
<connect gate="B1" pin="IO_L06N_1" pad="P73"/>
<connect gate="B1" pin="IO_L06P_1" pad="P72"/>
<connect gate="B1" pin="IP_1/VREF_1" pad="P68"/>
<connect gate="B1" pin="VCCO_1" pad="P67"/>
<connect gate="B2" pin="IO_2/MOSI/CSI_B" pad="P46"/>
<connect gate="B2" pin="IO_L01N_2/M0" pad="P25"/>
<connect gate="B2" pin="IO_L01P_2/M1" pad="P23"/>
<connect gate="B2" pin="IO_L02N_2/CSO_B" pad="P27"/>
<connect gate="B2" pin="IO_L02P_2/M2" pad="P24"/>
<connect gate="B2" pin="IO_L03N_2/VS1" pad="P30"/>
<connect gate="B2" pin="IO_L03P_2/RDWR_B" pad="P28"/>
<connect gate="B2" pin="IO_L04N_2/VS0" pad="P31"/>
<connect gate="B2" pin="IO_L04P_2/VS2" pad="P29"/>
<connect gate="B2" pin="IO_L05N_2/D7" pad="P34"/>
<connect gate="B2" pin="IO_L05P_2" pad="P32"/>
<connect gate="B2" pin="IO_L06N_2/D6" pad="P35"/>
<connect gate="B2" pin="IO_L06P_2" pad="P33"/>
<connect gate="B2" pin="IO_L07N_2/D4" pad="P37"/>
<connect gate="B2" pin="IO_L07P_2/D5" pad="P36"/>
<connect gate="B2" pin="IO_L08N_2/GCLK15" pad="P41"/>
<connect gate="B2" pin="IO_L08P_2/GCLK14" pad="P40"/>
<connect gate="B2" pin="IO_L09N_2/GCLK1" pad="P44"/>
<connect gate="B2" pin="IO_L09P_2/GCLK0" pad="P43"/>
<connect gate="B2" pin="IO_L10N_2/D3" pad="P49"/>
<connect gate="B2" pin="IO_L10P_2/INIT_B" pad="P48"/>
<connect gate="B2" pin="IO_L11N_2/D0/DIN/MISO" pad="P51"/>
<connect gate="B2" pin="IO_L11P_2/D2" pad="P50"/>
<connect gate="B2" pin="IO_L12N_2/CCLK" pad="P53"/>
<connect gate="B2" pin="IO_L12P_2/D1" pad="P52"/>
<connect gate="B2" pin="IP_2/VREF_2" pad="P39"/>
<connect gate="B2" pin="VCCO_2@0" pad="P26"/>
<connect gate="B2" pin="VCCO_2@1" pad="P45"/>
<connect gate="B3" pin="IO_L01N_3" pad="P4"/>
<connect gate="B3" pin="IO_L01P_3" pad="P3"/>
<connect gate="B3" pin="IO_L02N_3" pad="P6"/>
<connect gate="B3" pin="IO_L02P_3" pad="P5"/>
<connect gate="B3" pin="IO_L03N_3/LHCLK1" pad="P10"/>
<connect gate="B3" pin="IO_L03P_3/LHCLK0" pad="P9"/>
<connect gate="B3" pin="IO_L04N_3/IRDY2/LHCLK3" pad="P13"/>
<connect gate="B3" pin="IO_L04P_3/LHCLK2" pad="P12"/>
<connect gate="B3" pin="IO_L05N_3/LHCLK7" pad="P16"/>
<connect gate="B3" pin="IO_L05P_3/TRDY2/LHCLK6" pad="P15"/>
<connect gate="B3" pin="IO_L06N_3" pad="P20"/>
<connect gate="B3" pin="IO_L06P_3" pad="P19"/>
<connect gate="B3" pin="IP_3" pad="P21"/>
<connect gate="B3" pin="IP_3/VREF_3" pad="P7"/>
<connect gate="B3" pin="VCCO_3" pad="P11"/>
<connect gate="BGND" pin="GND@0" pad="P8"/>
<connect gate="BGND" pin="GND@1" pad="P14"/>
<connect gate="BGND" pin="GND@10" pad="P87"/>
<connect gate="BGND" pin="GND@11" pad="P91"/>
<connect gate="BGND" pin="GND@12" pad="P95"/>
<connect gate="BGND" pin="GND@2" pad="P18"/>
<connect gate="BGND" pin="GND@3" pad="P42"/>
<connect gate="BGND" pin="GND@4" pad="P47"/>
<connect gate="BGND" pin="GND@5" pad="P58"/>
<connect gate="BGND" pin="GND@6" pad="P63"/>
<connect gate="BGND" pin="GND@7" pad="P69"/>
<connect gate="BGND" pin="GND@8" pad="P74"/>
<connect gate="BGND" pin="GND@9" pad="P80"/>
<connect gate="BVCCAUX" pin="DONE" pad="P54"/>
<connect gate="BVCCAUX" pin="PROG_B" pad="P100"/>
<connect gate="BVCCAUX" pin="TCK" pad="P76"/>
<connect gate="BVCCAUX" pin="TDI" pad="P2"/>
<connect gate="BVCCAUX" pin="TDO" pad="P75"/>
<connect gate="BVCCAUX" pin="TMS" pad="P1"/>
<connect gate="BVCCAUX" pin="VCCAUX@0" pad="P22"/>
<connect gate="BVCCAUX" pin="VCCAUX@1" pad="P55"/>
<connect gate="BVCCAUX" pin="VCCAUX@2" pad="P92"/>
<connect gate="BVCCINT" pin="VCCINT@0" pad="P17"/>
<connect gate="BVCCINT" pin="VCCINT@1" pad="P38"/>
<connect gate="BVCCINT" pin="VCCINT@2" pad="P66"/>
<connect gate="BVCCINT" pin="VCCINT@3" pad="P81"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="xilinx_devices" deviceset="XC3S50AVQ100" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U1" gate="B0" x="-5.08" y="78.74"/>
<instance part="U1" gate="B1" x="-5.08" y="30.48"/>
<instance part="U1" gate="B2" x="-5.08" y="-33.02"/>
<instance part="U1" gate="B3" x="-5.08" y="-99.06"/>
<instance part="U1" gate="BGND" x="-5.08" y="-142.24"/>
<instance part="U1" gate="BVCCAUX" x="-5.08" y="-180.34"/>
<instance part="U1" gate="BVCCINT" x="-5.08" y="-205.74"/>
</instances>
<busses>
</busses>
<nets>
<net name="ADC1" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L02N_2/CSO_B"/>
<wire x1="0" y1="-15.24" x2="30.48" y2="-15.24" width="0.1524" layer="91"/>
<label x="22.86" y="-15.24" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC2" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L03P_2/RDWR_B"/>
<wire x1="0" y1="-17.78" x2="30.48" y2="-17.78" width="0.1524" layer="91"/>
<label x="22.86" y="-17.78" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC4" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L03N_2/VS1"/>
<wire x1="0" y1="-20.32" x2="30.48" y2="-20.32" width="0.1524" layer="91"/>
<label x="22.86" y="-20.32" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC3" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L04P_2/VS2"/>
<wire x1="0" y1="-22.86" x2="30.48" y2="-22.86" width="0.1524" layer="91"/>
<label x="22.86" y="-22.86" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC5" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L04N_2/VS0"/>
<wire x1="0" y1="-25.4" x2="30.48" y2="-25.4" width="0.1524" layer="91"/>
<label x="22.86" y="-25.4" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC6" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L05N_2/D7"/>
<wire x1="0" y1="-30.48" x2="30.48" y2="-30.48" width="0.1524" layer="91"/>
<label x="22.86" y="-30.48" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC7" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L06N_2/D6"/>
<wire x1="0" y1="-35.56" x2="30.48" y2="-35.56" width="0.1524" layer="91"/>
<label x="22.86" y="-35.56" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC8" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L07P_2/D5"/>
<wire x1="0" y1="-38.1" x2="30.48" y2="-38.1" width="0.1524" layer="91"/>
<label x="22.86" y="-38.1" size="1.778" layer="95"/>
</segment>
</net>
<net name="CROSS_HI" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L03P_3/LHCLK0"/>
<wire x1="0" y1="-93.98" x2="30.48" y2="-93.98" width="0.1524" layer="91"/>
<label x="22.86" y="-93.98" size="1.778" layer="95"/>
</segment>
</net>
<net name="MISO" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L05N_1"/>
<wire x1="0" y1="20.32" x2="30.48" y2="20.32" width="0.1524" layer="91"/>
<label x="22.86" y="20.32" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC_CLK" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L01P_3"/>
<wire x1="0" y1="-83.82" x2="30.48" y2="-83.82" width="0.1524" layer="91"/>
<label x="22.86" y="-83.82" size="1.778" layer="95"/>
</segment>
</net>
<net name="ADC_NOE" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L01N_3"/>
<wire x1="0" y1="-86.36" x2="30.48" y2="-86.36" width="0.1524" layer="91"/>
<label x="22.86" y="-86.36" size="1.778" layer="95"/>
</segment>
</net>
<net name="1356_CRYSTAL" class="0">
<segment>
<pinref part="U1" gate="B0" pin="IO_L04N_0/GCLK9"/>
<wire x1="0" y1="73.66" x2="30.48" y2="73.66" width="0.1524" layer="91"/>
<label x="22.86" y="73.66" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="U1" gate="B0" pin="IO_0/GCLK11"/>
<wire x1="0" y1="93.98" x2="30.48" y2="93.98" width="0.1524" layer="91"/>
<label x="22.86" y="93.98" size="1.778" layer="95"/>
</segment>
</net>
<net name="CROSS_LO" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L03N_3/LHCLK1"/>
<wire x1="0" y1="-96.52" x2="30.48" y2="-96.52" width="0.1524" layer="91"/>
<label x="22.86" y="-96.52" size="1.778" layer="95"/>
</segment>
</net>
<net name="DBG" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L04P_1/IRDY1/RHCLK6"/>
<wire x1="0" y1="27.94" x2="30.48" y2="27.94" width="0.1524" layer="91"/>
<label x="22.86" y="27.94" size="1.778" layer="95"/>
</segment>
</net>
<net name="MOSI" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L05P_1"/>
<wire x1="0" y1="22.86" x2="30.48" y2="22.86" width="0.1524" layer="91"/>
<label x="22.86" y="22.86" size="1.778" layer="95"/>
</segment>
</net>
<net name="NCS" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L04N_1/RHCLK7"/>
<wire x1="0" y1="25.4" x2="30.48" y2="25.4" width="0.1524" layer="91"/>
<label x="22.86" y="25.4" size="1.778" layer="95"/>
</segment>
</net>
<net name="PCK0" class="0">
<segment>
<pinref part="U1" gate="B0" pin="IO_L03N_0/GCLK7"/>
<wire x1="0" y1="78.74" x2="30.48" y2="78.74" width="0.1524" layer="91"/>
<label x="22.86" y="78.74" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWR_HI" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L02P_3"/>
<wire x1="0" y1="-88.9" x2="30.48" y2="-88.9" width="0.1524" layer="91"/>
<label x="22.86" y="-88.9" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWR_LO" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L02N_3"/>
<wire x1="0" y1="-91.44" x2="30.48" y2="-91.44" width="0.1524" layer="91"/>
<label x="22.86" y="-91.44" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWR_OE1" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L04P_3/LHCLK2"/>
<wire x1="0" y1="-99.06" x2="30.48" y2="-99.06" width="0.1524" layer="91"/>
<label x="22.86" y="-99.06" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWR_OE2" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L04N_3/IRDY2/LHCLK3"/>
<wire x1="0" y1="-101.6" x2="30.48" y2="-101.6" width="0.1524" layer="91"/>
<label x="22.86" y="-101.6" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWR_OE3" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L05P_3/TRDY2/LHCLK6"/>
<wire x1="0" y1="-104.14" x2="30.48" y2="-104.14" width="0.1524" layer="91"/>
<label x="22.86" y="-104.14" size="1.778" layer="95"/>
</segment>
</net>
<net name="PWR_OE4" class="0">
<segment>
<pinref part="U1" gate="B3" pin="IO_L05N_3/LHCLK7"/>
<wire x1="0" y1="-106.68" x2="30.48" y2="-106.68" width="0.1524" layer="91"/>
<label x="22.86" y="-106.68" size="1.778" layer="95"/>
</segment>
</net>
<net name="SPCK" class="0">
<segment>
<pinref part="U1" gate="B0" pin="IO_L02P_0/GCLK4"/>
<wire x1="0" y1="86.36" x2="30.48" y2="86.36" width="0.1524" layer="91"/>
<label x="22.86" y="86.36" size="1.778" layer="95"/>
</segment>
</net>
<net name="SSP_CLK" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L03N_1/TRDY1/RHCLK3"/>
<wire x1="0" y1="30.48" x2="30.48" y2="30.48" width="0.1524" layer="91"/>
<label x="22.86" y="30.48" size="1.778" layer="95"/>
</segment>
</net>
<net name="SSP_DIN" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L03P_1/RHCLK2"/>
<wire x1="0" y1="33.02" x2="30.48" y2="33.02" width="0.1524" layer="91"/>
<label x="22.86" y="33.02" size="1.778" layer="95"/>
</segment>
</net>
<net name="SSP_DOUT" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L02N_1/RHCLK1"/>
<wire x1="0" y1="35.56" x2="30.48" y2="35.56" width="0.1524" layer="91"/>
<label x="22.86" y="35.56" size="1.778" layer="95"/>
</segment>
</net>
<net name="SSP_FRAME" class="0">
<segment>
<pinref part="U1" gate="B1" pin="IO_L02P_1/RHCLK0"/>
<wire x1="0" y1="38.1" x2="30.48" y2="38.1" width="0.1524" layer="91"/>
<label x="22.86" y="38.1" size="1.778" layer="95"/>
</segment>
</net>
<net name="RX" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L08P_2/GCLK14"/>
<wire x1="0" y1="-43.18" x2="30.48" y2="-43.18" width="0.1524" layer="91"/>
<label x="22.86" y="-43.18" size="1.778" layer="95"/>
</segment>
</net>
<net name="TX" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L08N_2/GCLK15"/>
<wire x1="0" y1="-45.72" x2="30.48" y2="-45.72" width="0.1524" layer="91"/>
<label x="22.86" y="-45.72" size="1.778" layer="95"/>
</segment>
</net>
<net name="UART_IO1" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L09P_2/GCLK0"/>
<wire x1="0" y1="-48.26" x2="30.48" y2="-48.26" width="0.1524" layer="91"/>
<label x="22.86" y="-48.26" size="1.778" layer="95"/>
</segment>
</net>
<net name="UART_IO2" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L09N_2/GCLK1"/>
<wire x1="0" y1="-50.8" x2="30.48" y2="-50.8" width="0.1524" layer="91"/>
<label x="22.86" y="-50.8" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_DONE" class="0">
<segment>
<pinref part="U1" gate="BVCCAUX" pin="DONE"/>
<wire x1="0" y1="-177.8" x2="30.48" y2="-177.8" width="0.1524" layer="91"/>
<label x="22.86" y="-177.8" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_NPROGRAM" class="0">
<segment>
<pinref part="U1" gate="BVCCAUX" pin="PROG_B"/>
<wire x1="0" y1="-180.34" x2="30.48" y2="-180.34" width="0.1524" layer="91"/>
<label x="22.86" y="-180.34" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_CCLK" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L12N_2/CCLK"/>
<wire x1="0" y1="-66.04" x2="30.48" y2="-66.04" width="0.1524" layer="91"/>
<label x="22.86" y="-66.04" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_DIN" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L11N_2/D0/DIN/MISO"/>
<wire x1="0" y1="-60.96" x2="30.48" y2="-60.96" width="0.1524" layer="91"/>
<label x="22.86" y="-60.96" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_INIT" class="0">
<segment>
<pinref part="U1" gate="B2" pin="IO_L10P_2/INIT_B"/>
<wire x1="0" y1="-53.34" x2="30.48" y2="-53.34" width="0.1524" layer="91"/>
<label x="22.86" y="-53.34" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_TDI" class="0">
<segment>
<pinref part="U1" gate="BVCCAUX" pin="TDI"/>
<wire x1="0" y1="-185.42" x2="30.48" y2="-185.42" width="0.1524" layer="91"/>
<label x="22.86" y="-185.42" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_TDO" class="0">
<segment>
<pinref part="U1" gate="BVCCAUX" pin="TDO"/>
<wire x1="0" y1="-187.96" x2="30.48" y2="-187.96" width="0.1524" layer="91"/>
<label x="22.86" y="-187.96" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_TMS" class="0">
<segment>
<pinref part="U1" gate="BVCCAUX" pin="TMS"/>
<wire x1="0" y1="-190.5" x2="30.48" y2="-190.5" width="0.1524" layer="91"/>
<label x="22.86" y="-190.5" size="1.778" layer="95"/>
</segment>
</net>
<net name="FPGA_TCK" class="0">
<segment>
<pinref part="U1" gate="BVCCAUX" pin="TCK"/>
<wire x1="0" y1="-182.88" x2="30.48" y2="-182.88" width="0.1524" layer="91"/>
<label x="22.86" y="-182.88" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>