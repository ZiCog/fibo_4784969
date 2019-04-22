fastfibo(0, 0) :- !.
fastfibo(1, 1) :- !.
fastfibo(N, X) :-
    N mod 2 =:= 0 ->
        K is N / 2,
        K1 is K - 1,
        fastfibo(K1, X1),
        fastfibo(K, X2),
        X is ((2 * X1) + X2) * X2
    ;
        K is (N + 1) / 2,
        K1 is K - 1,
        fastfibo(K, X1),
        fastfibo(K1, X2),
        X is (X1 * X1) + (X2 * X2).

fibo(N, X) :-
    N >= 0 ->
        fastfibo(N, X)
    ;
        NN is -N,
        fastfibo(NN, Y),
        (N mod 2 =:= 0 -> X is -Y; X is Y).


fibo(-5, X).


