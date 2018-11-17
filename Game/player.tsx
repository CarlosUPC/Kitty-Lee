<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="player" tilewidth="32" tileheight="32" spacing="32" margin="15" tilecount="256" columns="16">
 <properties>
  <property name="animationSpeed" type="float" value="10"/>
  <property name="crashSound" value="audio/fx/jump_07.wav"/>
  <property name="incrementSpeedX" type="float" value="25"/>
  <property name="jumpSound" value="audio/fx/jump_03.wav"/>
  <property name="jumpSpeed" type="float" value="-800"/>
  <property name="maxSpeedX" type="float" value="80"/>
  <property name="walkingSound" value="audio/fx/Walk_Sound02.wav"/>
 </properties>
 <image source="textures/Player.png" width="1024" height="1024"/>
 <tile id="0">
  <objectgroup draworder="index">
   <object id="4" name="Collider" type="COLLIDER_PLAYER" x="9" y="2" width="14" height="30"/>
   <object id="7" name="ColliderDown" type="COLLIDER_PLAYER_DOWN" x="14" y="31" width="4" height="1"/>
   <object id="8" name="ColliderLeft" type="COLLIDER_PLAYER_LEFT" x="9" y="12" width="1" height="11"/>
   <object id="9" name="ColliderRight" type="COLLIDER_PLAYER_RIGHT" x="22" y="12" width="1" height="11"/>
   <object id="10" name="ColliderUp" type="COLLIDER_PLAYER_UP" x="15" y="2" width="1" height="1"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
  </animation>
 </tile>
 <tile id="4">
  <animation>
   <frame tileid="4" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
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
 <tile id="24">
  <animation>
   <frame tileid="24" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="26" duration="100"/>
   <frame tileid="27" duration="100"/>
   <frame tileid="28" duration="100"/>
   <frame tileid="29" duration="100"/>
   <frame tileid="30" duration="100"/>
   <frame tileid="31" duration="100"/>
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
 <tile id="40">
  <animation>
   <frame tileid="40" duration="100"/>
   <frame tileid="42" duration="100"/>
  </animation>
 </tile>
 <tile id="43">
  <animation>
   <frame tileid="43" duration="100"/>
  </animation>
 </tile>
 <tile id="44">
  <animation>
   <frame tileid="45" duration="100"/>
   <frame tileid="46" duration="100"/>
   <frame tileid="47" duration="100"/>
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
