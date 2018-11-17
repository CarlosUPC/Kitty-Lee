<?xml version="1.0" encoding="UTF-8"?>
<tileset name="gladiator" tilewidth="32" tileheight="32" tilecount="40" columns="8">
 <properties>
  <property name="AnimationSpeed" type="float" value="0.6"/>
 </properties>
 <image source="textures/enemies/Gladiator.png" width="256" height="160"/>
 <tile id="0">
  <objectgroup draworder="index">
   <object id="1" name="Collider" type="COLLIDER_ENEMY" x="4" y="5" width="24" height="27"/>
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
