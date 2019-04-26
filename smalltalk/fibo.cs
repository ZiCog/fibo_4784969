!Integer methodsFor: 'Integer'!
    fibo
		| k fk fk1 |
        (self < 2)
            ifTrue: [
                ^ self ]
            ifFalse: [
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
            ].
    
!

!
[
    "Required to work around file stream initialisation bug in Squeak 5.2"
    FileStream initialize.

    FileStream startUp: true.
    FileStream stdout nextPutAll: 4784969 fibo printString.
    FileStream stdout cr.
    FileStream stdout close.
    Smalltalk snapshot: false andQuit: true.
]
"Catch file I/O exceptions here else Squeak will hang when running headless and stdout is piped into 'head'"
on: Exception do:
[
    :x |
    Smalltalk snapshot: false andQuit: true.
]
!

