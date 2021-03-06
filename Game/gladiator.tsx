<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="gladiator" tilewidth="32" tileheight="32" tilecount="40" columns="8">
 <properties>
  <property name="AnimationSpeed" type="float" value="15"/>
  <property name="attackSound" value="audio/fx/sword_effect.wav"/>
 </properties>
 <image source="textures/entities/enemies/Gladiator.png" width="256" height="160"/>
 <tile id="0">
  <objectgroup draworder="index">
   <object id="1" name="Collider" type="COLLIDER_ENEMY" x="9" y="10" width="13" height="22"/>
   <object id="2" name="ColliderGround" type="COLLIDER_PLAYER_DOWN" x="13" y="31" width="6" height="1"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="4" duration="100"/>
  </animation>
 </tile>
 <tile id="15">
  <animation>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="12" duration="100"/>
   <frame tileid="13" duration="100"/>
   <frame tileid="14" duration="100"/>
   <frame tileid="15" duration="100"/>
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
  </animation>
 </tile>
 <tile id="24">
  <animation>
   <frame tileid="24" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="26" duration="100"/>
  </animation>
 </tile>
 <tile id="32">
  <animation>
   <frame tileid="32" duration="100"/>
   <frame tileid="33" duration="100"/>
   <frame tileid="34" duration="100"/>
   <frame tileid="35" duration="100"/>
   <frame tileid="36" duration="100"/>
   <frame tileid="37" duration="100"/>
   <frame tileid="38" duration="100"/>
  </animation>
 </tile>
</tileset>
