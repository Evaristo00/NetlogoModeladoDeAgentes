globals [
  hour
  count-down
  danger-time?
]

breed [ people person ]
breed [ thieves thief ]

turtles-own [ money direccion slow edad] ;both thieves and persons have
;direccion could be "der" "izq" "up" "down" "stay"

	to setup
  clear-all
  ask patches [
   set pcolor one-of [ grey]
  (foreach [16 8 0 -8 -16] [x ->
   if pxcor = x
    [ set pcolor green ]
   if pycor = x
    [ set pcolor green ]
      ])
  ]
  create-people CantPersonas [
		setxy  int random-xcor ((random 5 - 2) * 8)
		set money random 1000
    set edad random 100
    set slow 3 - random 3
    set direccion "stay"
    set color  white
    ifelse (money > 700)
    [set shape "person business"]
    [set shape "person"]
  ]
  create-thieves CantLadrones [
		setxy  int random-xcor ((random 4 - 2) * 8)
		set money  random 10
    set edad random 100
    set slow 3 - random 3
    set direccion "stay"
    set color  black
    ifelse (slow = 1)
    [set shape "bici 2"]
    [set shape "person"]

  ]
  set hour 0
  set danger-time? false
  Reset-ticks
end

to go

  if ticks mod 30 = 0 [ update-hour ]

  clear-output
  output-type "Current Time: " output-type hour  output-print ":00 HS"
  output-type "Dead People: " output-print CantPersonas - count people
  output-type "People Alive: " output-print count people
  output-type "Robbed People Alive: " output-print count people with [ color = red]

  output-type "People's Money: "  output-type sum [money] of people output-print " Pesos"
  output-type "Money Robbed: "  output-type sum [money] of thieves output-print " Pesos"


	;llamar a todos los procesos
  ask people [
    if ticks mod slow = 0 
    [
      cambiarDirec
      move
    ]
  ]
  ask thieves[
    if ticks mod slow = 0  
    [    
      move 
    ]
    steal
  ]
  tick
end

to update-hour
  ifelse hour < 24 [
    set hour hour + 1
  ]
  [ set hour 0 ]
  if (hour > -1) and (hour < 7) [ set danger-time? true] 
end

to move  ; turtle procedure
  let der patch-at -1 0
  let izq patch-at 1 0
  let up patch-at 0 1
  let down patch-at 0 -1
  let prob (list "der" "izq" "up" "down")
  if (izq = nobody) or ([pcolor] of izq != green) or (direccion = "der") [
    ;face izq
    set prob remove "izq" prob
  ]
  if (der = nobody) or ([pcolor] of der != green) or (direccion = "izq") [
     ;face der
    set prob remove "der" prob
  ]
  if (up = nobody) or ([pcolor] of up != green) or (direccion = "down") [
     set prob remove "up" prob
  ]
  if (down = nobody) or ([pcolor] of down != green) or (direccion = "up") [
     set prob remove "down" prob
  ]
  set direccion one-of prob
  ifelse direccion = "der"
    [ face der ]
  [ifelse direccion = "izq"
    [ face izq ]
    [ifelse direccion = "up"
      [ face up ]
      [face down]
    ]
  ]
  fd 1
end



to-report myMoney
  report money
end

to steal ; thief procedure



  let prey one-of people-here     ; grab a random prey from the spot
  let newMoney 0

  ;; did we get one? if so,
  if prey != nobody  [
    ;;set money [money] of prey ;;=>Another way of setting the money

    let prob 10
    if danger-time? [set prob prob + 50]
    if ( [edad] of prey > 18 ) [set prob prob + 10]
    if ( [edad] of prey > 50 ) [set prob prob + 10]
    if ( [money] of prey > 300 ) [set prob prob + 10]
    if ( [money] of prey > 700 ) [set prob prob + 10]

    if random 100 < prob [

      ask prey [
        ifelse money = 0 [ die ]
        [
          set newMoney money
          set money 0
          set color  red
        ]
      ]
      set money money + newMoney
    ]

  ]




end


to cambiarDirec
   (ifelse
    direccion = "der" [
      (foreach [-1 -2 -3 ] [x ->
        if count thieves-at x 0 != 0
        [set direccion "izq"]
      ])
    ]
    direccion = "izq" [
      (foreach [1 2 3 ] [x ->
        if count thieves-at x 0 != 0
        [set direccion "der"]
      ])
    ]
    direccion = "up" [
      (foreach [1 2 3 ] [x ->
        if count thieves-at 0 x != 0
        [set direccion "down"]
      ])
    ]
    ; elsecommands down
    [
      (foreach [-1 -2 -3 ] [x ->
        if count thieves-at 0 x != 0
        [set direccion "up"]
      ])
  ])
end


