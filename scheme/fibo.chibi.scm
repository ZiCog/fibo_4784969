;#!/usr/bin/racket
;#lang racket

(import (scheme base))
(import (scheme write))
(import (scheme inexact))

; Fibonacci, only intger exact up to fibo 70
(define fiboInexact (lambda (n)
    (let (
        (alpha (/  (+ 1 (sqrt 5)) 2) )
        (beta (/  (- 1 (sqrt 5)) 2) )
         ) 

         (round (- (* (/ 1 (sqrt 5)) (expt alpha n)) 
         (* (/ 1 (sqrt 5)) (expt beta n))))
    )
))

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
                            (aa (* 2 a))
                            (c (* (+ aa b) (- aa b)))
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



;(display (fiboInexact 70))
;(newline)


(let
    (
        (f (fibo 70))
    )
    (display f)
    (newline)
)

