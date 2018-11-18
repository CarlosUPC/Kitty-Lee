<?xml version="1.0" encoding="UTF-8"?>
<tileset name="Flying Tongue" tilewidth="64" tileheight="64" tilecount="32" columns="8">
 <properties>
  <property name="AnimationSpeed" type="float" value="15"/>
 </properties>
 <image source="textures/enemies/FlyingTongue.png" width="512" height="256"/>
 <tile id="0">
  <objectgroup draworder="index">
   <object id="1" name="Collider" type="COLLIDER_ENEMY" x="5" y="9" width="49" height="44"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="4" duration="100"/>
  </animation>
 </tile>
 <tile id="8">
  <animation>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="12" duration="100"/>
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
</tileset>
