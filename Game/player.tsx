<?xml version="1.0" encoding="UTF-8"?>
<tileset name="player" tilewidth="32" tileheight="32" spacing="32" margin="15" tilecount="256" columns="16">
 <properties>
  <property name="animationSpeed" type="float" value="0.2"/>
  <property name="colliderHeight" type="int" value="29"/>
  <property name="colliderOffsetX" type="int" value="10"/>
  <property name="colliderOffsetY" type="int" value="3"/>
  <property name="colliderWidth" type="int" value="12"/>
  <property name="incrementSpeedX" type="float" value="1"/>
  <property name="jumpSound" value="audio/fx/jump_03.wav"/>
  <property name="jumpSpeed" type="float" value="-8"/>
  <property name="maxSpeedX" type="float" value="2"/>
  <property name="walkingSound" value="audio/fx/Walk_Sound02.wav"/>
 </properties>
 <image source="textures/Player.png" width="1024" height="1024"/>
 <tile id="0">
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
  </animation>
 </tile>
 <tile id="16">
  <animation>
   <frame tileid="16" duration="100"/>
   <frame tileid="17" duration="100"/>
   <frame tileid="18" duration="100"/>
   <frame tileid="19" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="21" duration="100"/>
   <frame tileid="22" duration="100"/>
   <frame tileid="23" duration="100"/>
  </animation>
 </tile>
 <tile id="32">
  <animation>
   <frame tileid="32" duration="100"/>
   <frame tileid="34" duration="100"/>
  </animation>
 </tile>
 <tile id="35">
  <animation>
   <frame tileid="35" duration="100"/>
  </animation>
 </tile>
 <tile id="36">
  <animation>
   <frame tileid="37" duration="100"/>
   <frame tileid="38" duration="100"/>
   <frame tileid="39" duration="100"/>
  </animation>
 </tile>
 <tile id="64">
  <animation>
   <frame tileid="64" duration="100"/>
   <frame tileid="65" duration="100"/>
   <frame tileid="66" duration="100"/>
   <frame tileid="67" duration="100"/>
   <frame tileid="68" duration="100"/>
   <frame tileid="69" duration="100"/>
   <frame tileid="70" duration="100"/>
   <frame tileid="71" duration="100"/>
   <frame tileid="72" duration="100"/>
  </animation>
 </tile>
 <tile id="80">
  <animation>
   <frame tileid="80" duration="100"/>
   <frame tileid="81" duration="100"/>
   <frame tileid="82" duration="100"/>
   <frame tileid="83" duration="100"/>
   <frame tileid="84" duration="100"/>
   <frame tileid="85" duration="100"/>
   <frame tileid="86" duration="100"/>
  </animation>
 </tile>
 <tile id="96">
  <animation>
   <frame tileid="96" duration="100"/>
   <frame tileid="97" duration="100"/>
   <frame tileid="98" duration="100"/>
   <frame tileid="99" duration="100"/>
   <frame tileid="100" duration="100"/>
   <frame tileid="101" duration="100"/>
  </animation>
 </tile>
</tileset>
