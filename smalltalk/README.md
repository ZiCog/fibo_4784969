Gentlemen,

After a long and difficult research and development period Heater Labs is proud to announce a world first, a break though in software engineering that will upend your very conception of what computing means. 

In short the boffins at heater labs have managed to compute the first Fibonacci number of one million digits using the Smalltalk language, run from the command line, starting from a regular text file of source code. This task, which many said was impossible, can now be accomplished by anyone thanks to the open nature of the Heater Labs. See below for details. 

This magnificent acheivement did not come with out significant personal sacrifice by the team at Heater Labs. Especially the chief of reasearch who did not sleep for a week whilst tackling this seemingly impossible task. He is now receiving the therapy he needs and his family prays for a full recovery.

Here are the results, first the code:

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

And the outcome:

    $ time gst -g fibo.st
    ...951766313865956891695455563801462249978567353436540666740580717167763216988216644330074030719891463180149736853685001275152076875379936330930391815964864885353407167474856539211500699706378405156269

    real    311m54.607s
    user    297m4.313s
    sys     13m47.016s
    $

Yes folks, I think you will agree this is a breath taking result. The full one million digits of computing fibo(4784969) in only five hours and 12 minutes.

How was this incredible feat acheived you might ask?

Fear not, the research of Heater Labs is free and open to everyone. Technical details will be published soon.















