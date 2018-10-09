<?xml version="1.0" encoding="UTF-8"?>
<tileset name="player" tilewidth="32" tileheight="32" spacing="32" margin="15" tilecount="256" columns="16">
 <image source="textures/Player.png" width="1024" height="1024"/>
 <tile id="0">
  <animation name="idle">
   <frame tileid="0" duration="200"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
  </animation>
 </tile>
 <tile id="16">
  <animation name="walking_right">
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
   <frame tileid="33" duration="100"/>
   <frame tileid="34" duration="100"/>
   <frame tileid="35" duration="100"/>
   <frame tileid="36" duration="100"/>
   <frame tileid="37" duration="100"/>
   <frame tileid="38" duration="100"/>
   <frame tileid="39" duration="100"/>
  </animation>
 </tile>
 <tile id="48">
  <animation>
   <frame tileid="48" duration="100"/>
   <frame tileid="49" duration="100"/>
   <frame tileid="50" duration="100"/>
   <frame tileid="51" duration="100"/>
   <frame tileid="52" duration="100"/>
   <frame tileid="53" duration="100"/>
   <frame tileid="54" duration="100"/>
   <frame tileid="55" duration="100"/>
   <frame tileid="56" duration="100"/>
   <frame tileid="57" duration="100"/>
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
</tileset>
<data>
	<speed incrementSpeedX="0.005f" gravity="0.009f" maxSpeedX="0.25f" jump="-1.0f"/>
</data>