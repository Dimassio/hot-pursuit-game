﻿# Игра "Тараканьи бега"


Задача: реализовать движок игры

Команда:

 - Звонарев Дмитрий
 - Никифорова Ульяна
 - Жидков Антон

Особенности:

 - Стартовая линия совпадает с финишем, и игроки стартуют непосредственно со стартовой линии.
 - Оказалось, что ситуация, когда игрок стоит на старте(на финише) и несколько ходов остается на месте, плохо обработана. Реализовано только для 2х ходов :)
 - Есть интерфейс в CGame, передающий фронтенду координаты точек для отрисовки.
 - AI, при использовании в игре, должен быть добавлен в players[], и в start() в вызове turnOfPlayer(size_t numOfPlayer) нужно передать AI конфигурацию хода, а в замен он отдаст результат - его ход.