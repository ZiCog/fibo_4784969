Integer extend [
    fibo [  | k fk fk1 | 
        (self < 2)
            ifTrue: [
                ^ self ].
        (self = 2)
            ifTrue: [
                ^ 1
            ].
        (self > 2)
            ifTrue: [
                k := (self + 1) // 2. 
                fk := k fibo.
                fk1 := (k - 1) fibo.
                (self odd)
                    ifTrue: [
                        ^ (fk * fk) + (fk1 * fk1)
                    ]
                    ifFalse: [
                        ^ ((2 * fk1) + fk) * fk
                    ]
            ]
    ]
]

Integer extend [
    fibIterative [ | a b |
        a := b := 1.
        (self - 1) timesRepeat: [
            a := b + (b := a)
        ].
        ^b
    ]
]

4784969 fibo printNl.

