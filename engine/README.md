# Computer Graphics
### CG engine
#### Ward Gauderis S0183431 - Uantwerpen bachelor 1 informatica

functionaliteit:
- (inleiding)
- 2D L-systemen met haakjes en stochastische replacement rules
- 3D lijntekeningen
- 3D lichamen en 3D L-systemen
- Z-buffering met lijnen
- Z-buffering met driehoeken
- 3D fractalen, buckyball en Menger spons
- ambient licht, diffuus licht (op oneindig en puntbronnen), speculair licht
- schaduwen en texture mapping
- 3D lijntekeningen met bollen en cilinders

extra/voorbeelden:
- stochastic L-systemen in 2D en 3D: zie voorbeelden in "tests"  
  alle regels die een kans maken, worden voorafgegaan door hun kans in de L2D/L3D file
- Klasse Lines2D verwijderdt dubbele lijnen automatisch (d.m.v. een set) wanneer 3d-figuren wordt omgezet
- repeterende textures op willekeurige oppervlakken met interpolatie: zie voorbeelden in de folder "tests"  
  de kleur van de texture wordt standaard gebruikt voor de ambiente, diffuse en speculaire kleurcomponent  
  (werkt zowel met als zonder schaduw)  
  om aan te tonen dat een figuur een texture heeft in de ini, moeten de velden "texture", "p", "a" en "b" (benamingen uit de cursus) aanwezig zijn in de sectie van die figuur
- wanneer thick figures worden gegenereerd, worden enkel de gebruikte edges en punten omgezet in cilinders en bollen (dubbels worden niet gegenereerd)
- backface culling voor driehoeken
