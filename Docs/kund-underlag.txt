# NordAir Systems AB

### SmartVent Monitor

**Vi är bäst i världen på blåsjobb**

Stockholm, 31 augusti 2026

## Förfrågan om utveckling av prototyp för SmartVent Monitor

Hej,

Mitt namn är **Lars Ventström** och jag är VD och medgrundare för **NordAir Systems AB**.

NordAir Systems är ett nystartat teknikföretag som arbetar med övervakning och digitalisering av ventilationssystem. Vår vision är att göra ventilationsanläggningar enklare att övervaka, underhålla och felsöka genom att kombinera sensorteknik, embedded-system och IoT.

Vår filosofi är enkel:

**Vi är bäst i världen på blåsjobb.**

För att kunna leva upp till detta behöver vi också kunna se när våra ventilationssystem inte längre fungerar som de ska.

Det är utifrån denna idé som vår första produkt, **SmartVent Monitor**, har vuxit fram.

---

## Vad är SmartVent Monitor?

SmartVent Monitor är tänkt att bli ett kompakt övervakningssystem för ventilationsaggregat.

Ventilationssystem är ofta byggda för att kunna vara i drift under långa perioder utan ständig övervakning. Problem kan därför hinna utvecklas under lång tid innan någon upptäcker dem.

Ett lager som börjar vibrera, en fläkt som tappar prestanda eller en temperatur som gradvis förändras kan vara tecken på ett underliggande problem.

Vi vill därför undersöka om ett IoT-baserat övervakningssystem kan användas för att kontinuerligt samla in information från en ventilationsenhet och upptäcka avvikande beteenden.

Den första prototypen behöver inte vara kopplad till ett riktigt ventilationsaggregat. En simulerad ventilationsenhet är fullt tillräcklig.

---

## Vad vill vi kunna mäta?

I den första prototypen vill vi kunna samla in åtminstone följande information:

* lufttemperatur,
* relativ luftfuktighet,
* vibrationer från ventilationsfläkten,
* fläktens aktuella driftstatus eller hastighet.

Mätningarna ska genomföras återkommande över tid.

Det räcker alltså inte att endast kunna läsa av ett aktuellt sensorvärde.

Vi vill kunna samla in historiska mätvärden så att systemet senare kan analysera hur ventilationsenheten beter sig över tid.

Ett framtida system skulle exempelvis kunna upptäcka att vibrationerna från en fläkt gradvis ökar. Detta skulle kunna vara en indikation på att något behöver undersökas innan ett faktiskt fel uppstår.

På motsvarande sätt skulle systemet kunna upptäcka temperaturer eller luftfuktighetsnivåer som avviker från definierade gränsvärden.

---

## Simulerad ventilationsenhet

Eftersom detta är en första prototyp behöver ni inte utveckla ett komplett kommersiellt ventilationsaggregat.

Vi vill istället att utvecklingsteamet bygger en mindre **simulerad ventilationsenhet** med hjälp av en mikrokontroller och relevanta sensorer.

Den simulerade enheten ska kunna representera en fungerande ventilationsfläkt och kunna generera sensorvärden.

Vi vill även att det ska vara möjligt att simulera olika typer av avvikelser.

Exempelvis:

* normalt fläktläge,
* för höga vibrationer,
* för låg eller hög temperatur,
* onormal luftfuktighet,
* förändrad fläkthastighet,
* bortfall eller felaktiga sensorvärden.

Det är viktigt för oss att kunna demonstrera att systemet inte bara samlar in data utan även kan användas för att identifiera potentiella problem.

---

## Våra tidigare erfarenheter

Det här är inte vårt första försök att digitalisera ventilationsövervakning.

I ett tidigare utvecklingsprojekt byggdes en enklare prototyp som kunde läsa av sensorer och visa aktuella värden.

Resultatet fungerade vid en kort demonstration men visade sig vara mindre användbart vid längre tester.

Framför allt saknades möjlighet att analysera historiska data och upptäcka förändringar över tid.

Vi upplevde även att systemet hade svårt att skilja mellan normala variationer och potentiellt felaktiga mätvärden.

NordAir Systems vill därför att den nya prototypen tar hänsyn till:

* datakvalitet,
* stabilitet,
* historiska mätvärden,
* felaktiga eller orimliga sensorvärden,
* möjligheten att vidareutveckla systemet.

Vi vill inte att komponenter väljs enbart för att de är enkla att få tag på. Valet av sensorer och övriga komponenter ska kunna motiveras utifrån projektets krav.

---

## Kommunikation och IoT

En central del av projektet är att sensorinformationen ska kunna lämna den lokala embedded-enheten.

Vi har ännu inte bestämt vilken kommunikationsteknik eller molnplattform som ska användas i en framtida produkt.

I den första prototypen vill vi därför att utvecklingsteamet undersöker och väljer en lämplig lösning.

Det kan exempelvis handla om:

* MQTT,
* HTTP/REST,
* annan lämplig IoT-kommunikation.

Det viktiga är att prototypen visar att en mikrokontroller kan samla in sensorvärden och kommunicera dessa till ett externt system.

Vi ser gärna att lösningen byggs på ett sådant sätt att flera ventilationsenheter kan anslutas i framtiden.

---

## Backend och databehandling

Vi vill att den mottagna informationen behandlas i ett backend-system.

Backend-systemet ska åtminstone kunna:

* ta emot sensordata,
* identifiera vilken enhet datan kommer från,
* lagra mätvärden,
* kontrollera om värden ligger inom definierade gränser,
* identifiera potentiella avvikelser,
* tillhandahålla data för presentation i ett gränssnitt.

Vi ser gärna att systemet använder en databas eftersom historiska mätvärden kommer att vara viktiga för framtida analys.

---

## Dashboard

Som en del av prototypen vill vi kunna se ventilationssystemets aktuella status.

Dashboarden bör exempelvis kunna visa:

* aktuell temperatur,
* aktuell luftfuktighet,
* aktuell vibrationsnivå,
* fläktstatus,
* senaste uppdatering,
* systemets aktuella status.

Exempel på status kan vara:

**NORMAL**

**VARNING**

**KRITISK**

Vi vill även kunna se om systemet har upptäckt en avvikelse.

En framtida version skulle kunna innehålla grafer över historiska sensorvärden och mer avancerad analys.

---

## Vad vi förväntar oss av utvecklingsteamet

Vi betraktar er som det utvecklingsteam som fått ansvar för projektets första tekniska prototyp.

Vi förväntar oss därför inte bara fungerande kod.

Vi vill kunna förstå:

* hur ni har tolkat våra behov,
* vilka krav ni har identifierat,
* hur systemet är arkitekturerat,
* vilka tekniska beslut ni har fattat,
* varför ni har valt dessa tekniker,
* hur sensorer och mikrokontroller kommunicerar,
* hur IoT-kommunikationen fungerar,
* hur data lagras,
* hur avvikelser identifieras,
* hur lösningen har testats,
* vilka begränsningar prototypen har,
* hur systemet skulle kunna vidareutvecklas.

Vi är medvetna om att detta är en första prototyp.

Vi förväntar oss därför inte en färdig kommersiell produkt.

En mindre lösning som fungerar stabilt och där tekniska beslut är välmotiverade är betydligt mer värdefull för oss än en stor lösning där många funktioner endast fungerar delvis.

---

## Några frågor som vi hoppas att ni hjälper oss att besvara

När projektet är färdigt hoppas vi bland annat ha bättre svar på följande:

* Vilka sensorer är lämpliga för övervakning av en ventilationsenhet?
* Hur ofta bör sensordata samlas in?
* Vilket kommunikationsprotokoll är lämpligt?
* Hur bör sensorinformationen struktureras?
* Hur kan systemet upptäcka orimliga eller felaktiga mätvärden?
* Hur kan vibrationer användas för att upptäcka potentiella problem med en fläkt?
* Hur bör historiska mätdata lagras?
* Hur kan systemet skalas från en till flera ventilationsenheter?
* Hur bör en framtida produkt byggas för att enkelt kunna vidareutvecklas?
* Vilka risker och begränsningar ser utvecklingsteamet i den föreslagna lösningen?

Vissa av dessa frågor kanske går att besvara redan i prototypen.

Andra kanske snarare leder till rekommendationer inför nästa utvecklingsfas.

Båda resultaten är värdefulla för oss.

Vi ser fram emot att se hur ni angriper problemet och hur den första generationen av **SmartVent Monitor** kan ta form.

Med vänliga hälsningar,

**Lars Ventström**
VD och medgrundare
**NordAir Systems AB**

*Vi är bäst i världen på blåsjobb.*

**NordAir Systems AB – SmartVent Monitor Development Initiative**