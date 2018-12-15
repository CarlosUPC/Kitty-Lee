<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="Flying Tongue" tilewidth="64" tileheight="64" tilecount="24" columns="8">
 <properties>
  <property name="AnimationSpeed" type="float" value="15"/>
  <property name="attackSound" value="audio/fx/bite_effect.wav"/>
 </properties>
 <image source="textures/entities/enemies/Flying_Tongue.png" width="512" height="192"/>
 <tile id="0">
  <objectgroup draworder="index">
   <object id="1" name="Collider" type="COLLIDER_ENEMY" x="21" y="22" width="23" height="26"/>
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
