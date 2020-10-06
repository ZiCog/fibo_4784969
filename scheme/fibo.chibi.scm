(import (scheme base))
(import (scheme write))
(import (scheme inexact))
(import (srfi 69))       ; Basic hash tables

(define memo (make-hash-table eq?))

(define (memoize n f)
    (hash-table-set! memo n f)
    f)

(define (memoized n)
    (hash-table-exists? memo n))

(define (memoref n)
    (hash-table-ref memo n))

(memoize 0 0)
(memoize 1 1)

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

(define (fiboCalc n)
    (cond
        ((= n 0) 0)
        ((= n 1) 1)
        (else  

            (let* (
                (k (quotient n 2))
                (a (fibo k))
                (b (fibo (- k 1))))

                (if (even? n)
                    (memoize n (* a (+ (+ b b) a)))
                    (let* (
                        (twoa (+ a a))
                        (c (* (+ twoa b) (- twoa b))))

                        (if (= (modulo n 4) 1)
                            (memoize n (+ c 2))
                            (memoize n (- c 2))
                        )
                    )
                )
            )
        )
    )
)

(define (fibo n)
    (if (memoized n)
        (memoref n)
        (fiboCalc n)
    )
)

(let (
    (f (fibo 4784969)))
    (display f)
    (newline)
)

