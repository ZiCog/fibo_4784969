
; Fibonacci, only integer exact up to fibo 70
(define fiboInexact (lambda (n)
    (let (
        (alpha (/  (+ 1 (sqrt 5)) 2) )
        (beta (/  (- 1 (sqrt 5)) 2) )
         ) 

         (round (- (* (/ 1 (sqrt 5)) (expt alpha n)) 
         (* (/ 1 (sqrt 5)) (expt beta n))))
    )
))

; Fibonacci using fast doubling formulas, good for millions of digits!
(define (fibo n)
    (cond
        ((= n 0) 0)
        ((= n 1) 1)
        (else  

            (let*
                (
                    (k (quotient n 2))
                    (a (fibo k))
                    (b (fibo (- k 1)))
                )

                (if (even? n)
                    (* a (+ (* 2 b) a))
                    (let*
                        (
                            (2a (* 2 a))
                            (c (* (+ 2a b) (- 2a b)))
                        )
    
                        (if (= (modulo n 4) 1)
                            (+ c 2)
                            (- c 2)
                        )
                    )
                )
            )
        )
    )
)

(let
    (
        (f (fibo 4784969))
    )
    (display f)
    (newline)
)

