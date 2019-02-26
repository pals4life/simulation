//#include <iostream>
//#include <queue>
//
//
//
//class Road
//{
//public:
//    Road(const std::string& name) : kfName(name) {}
//
//    void addVehicle(IVehicle* vehicle)
//    {
//        fVehicles.push(vehicle);
//    }
//
//    void update()
//    { a
////        1.  Bereken nieuwe positie van voertuig
////        2.  Bereken nieuwe snelheid van voertuig
////        3.  Bereken nieuwe versnelling van voertuig
//
////        4.  IF nieuwe positie valt buiten huidige baan
////        4.1.  IF huidige baan heeft verbinding
////        4.1.1 Zet voertuig op verbindingsbaan
////        4.2.  ELSE
////        4.2.1.  Verwijder voertuig uit simulatie
//    }
//
//private:
//    const std::string kfName;
//    const Road* kfConnection;
//
//    std::queue<IVehicle*> fVehicles;
//};
//
//class Network
//{
//public:
//    explicit Network() {}
//
//    void simulate()
//    {
////        1.  WHILE voertuigen in het wegennetwerk
////        1.1 FOR elk voertuig in het wegennetwerk
////        1.1.1 voer use case 3.1 uit op het voertuig
//    }
//
//    void output()
//    {
////        1.  Open uitvoerbestand
////        2.  WHILE Nog banen beschikbaar
////        2.1.  Schrijf baan-gegevens uit
////        3.  WHILE Nog voertuigen beschikbaar
////        3.1.  Schrijf voertuig-gegevens uit
////        4.  Sluit uitvoerbestand
//    }
//
//private:
//    std::vector<IVehicle*> vehicles;
//    std::vector<Road*> roads;
//};
//
//class NetworkParer
//{
//public:
//    static Network parse(const std::string& path)
//    {
////        1.  Open invoerbestand
////        2.  WHILE Bestand niet ingelezen
////        2.1.  Herken het soort element (VOERTUIG, BAAN)
////        2.2.  Lees verdere informatie voor het element
////        2.3.  IF Verifieer geldige informatie
////        2.3.1.  THEN Voeg element toe aan de simulatie
////        2.3.1.  ELSE Foutboodschap + positioneer op volgende element in het bestand
////        3.  Verifieer consistentie van de verkeerssituatie
////        4.  Sluit invoerbestand
//
////        2.1.  [Onherkenbaar element] Foutboodschap + positioneer op volgende element in het be-stand ⇒ verdergaan vanaf stap 2
////        2.2.  [Ongeldige informatie] Foutboodschap + positioneer op volgende element in het be-stand ⇒ verdergaan vanaf stap 2
////        3.  [Inconsistente verkeerssituatie] Foutboodschap⇒verdergaan vanaf stap 4
//
////        Wij gaan hier ook elk voertuig al in zijn juiste datastructuur steken.
//        return Network();
//    }
//};
//
//
//int main()
//{
//    std::cout << "Hello World" << std::endl;
//    return 0;
//}