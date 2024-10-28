# Botul „Popa Șchiopu”

Acest bot de Halite este inspirat de [această idee de soluție](http://braino.org/thoughts/halite_the_simple_way.html) de la competiția oficială.

## Descriere sumară a soluției

Determinăm pentru marginile externe (celulele care nu sunt controlate de noi) ale teritoriului controlat de noi un _factor_ de dezirabilitate pentru fiecare celulă margine externă, `producțiaCelulei * OUTER_PRODUCTION_SCALE - putereaCelulei`.

Pentru fiecare celulă din interior, determinăm vecinul cu cel mai mare _factor_, și setăm factorul celulei din interior cu `factorulCeluleiVecine - producțiaCeluleiInterioare * INNER_PRODUCTION_SCALE` (reflectăm pierderea de producție a celulei în rândul în care o mutăm în _factor_).
Acest lucru se poate implementa cu o coadă cu priorități, sortată după factorul celulelor, în care punem inițial celulele din marginea exterioară cu _factorul_ lor aferent, iar apoi, pe măsură ce extragem din el celule, le setăm vecinilor ce ne aparțin _factorul_ și direcția propusă în rândul curent spre celula extrasă (avem grijă să nu depășim limita de `255 + ACCEPTED_STRENGTH_LOSS` în celula destinație; pentru asta ținem o matrice cu puterile ce ar rezulta în urma mutărilor propuse pentru celulele din joc; putem propune ca o celulă să se mute spre un vecin care nu are cel mai bun _factor_ dacă în acela nu ar rezulta acea depășire de limită de putere) și îi adăugăm în coada cu priorități.

Într-un final, prelucrăm mutările propuse anterior astfel:
- Dacă puterea celulei e mai mică decât `producțiaCelulei * TURNS_BEFORE_LEAVING`, atuci stă pe loc (un rând în care mutăm celula e unul în care ea nu produce, deci nu e bine să mutăm celule slabe doar de dragul de a le muta).
- Dacă celula are adiacentă una care nu e a noastră cu putere mai mică, o capturăm.
- Dacă ne-am fi îndreptat spre o celulă care nu e a noastră de putere mai mare, stăm pe loc.

Pentru acest algoritm sunt folosite următoarele constante, determinate prin încercări să reușească etapa 2:
- `OUTER_PRODUCTION_SCALE = OUTER_PRODUCTION_SCALE_CONSTANT + lățimeaHărții * înălțimeaHărții * OUTER_PRODUCTION_SCALE_SCALE`: putem justifica scalarea după numărul de celule din hartă ca o recunoaștere a importanței mai mari a producției în cazul hărților mai mari, dar motivul pentru care nu am pus-o constantă simplă este că nu am reușit să găsesc una care să meargă și pentru cazul `28x24` (unde mergea `7`, printre alte câteva valori discrete), și `30x30` (unde mergea `7.5`, printre alte câteva valori discrete).
    - `OUTER_PRODUCTION_SCALE_CONSTANT = 5.656`
    - `OUTER_PRODUCTION_SCALE_SCALE = 0.002`
- `INNER_PRODUCTION_SCALE = 2`
- `TURNS_BEFORE_LEAVING = 4`
- `ACCEPTED_STRENGTH_LOSS = 50`
