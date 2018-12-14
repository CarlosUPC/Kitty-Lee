<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="coin" tilewidth="32" tileheight="32" spacing="10" margin="5" tilecount="9" columns="3">
 <image source="textures/coin.png" width="126" height="126"/>
 <tile id="0">
  <objectgroup draworder="index">
   <object id="1" name="Collider" type="COLLIDER_COIN" x="6" y="6" width="20" height="20"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="150"/>
   <frame tileid="1" duration="150"/>
   <frame tileid="2" duration="150"/>
   <frame tileid="3" duration="150"/>
   <frame tileid="4" duration="150"/>
   <frame tileid="5" duration="150"/>
   <frame tileid="6" duration="150"/>
   <frame tileid="7" duration="150"/>
   <frame tileid="8" duration="150"/>
   <frame tileid="4" duration="150"/>
  </animation>
 </tile>
</tileset>
