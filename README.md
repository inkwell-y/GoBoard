# GoBoardController

Current Go rules milestone status:

- The game currently supports stone placement, captures, suicide prevention, ko via repeated-board-position rejection, pass, game over after two consecutive passes, and simple Chinese area scoring.
- The system does not automatically determine dead stones.
- Stones remaining on the board are counted as alive for scoring.
- For accurate scoring without dead-stone marking, players should continue playing until dead stones are actually captured.
- Manual dead-stone marking may be added later.

Current scoring notes:

- Black score = black stones on the board + empty regions surrounded only by Black.
- White score = white stones on the board + empty regions surrounded only by White + komi.
- Neutral empty regions bordered by both colors are not counted for either player.
