#!/usr/bin/racket
#lang racket

(define memo (make-hash))

(define (memoize n f)
    (hash-set! memo n f)
    f)

(define (memoized n)
    (dict-has-key? memo n))

(define (memoref n)
    (hash-ref memo n))

(hash-set! memo 0 0)
(hash-set! memo 1 1)

(define (fiboCalc n)
    (define k (quotient n 2))
    (define a (fibo k))
    (define b (fibo (- k 1)))
    (if (even? n)
        (memoize n (* a (+ (* 2 b) a)))
        (let ([c (* (+ (* 2 a) b) (- (* 2 a) b))])
            (if (= (modulo n 4) 1)
                (memoize n (+ c 2)) 
                (memoize n (- c 2))))))

(define (fibo n)
    (if (memoized n)
        (memoref n)
        (fiboCalc n)))

(fibo 4784969)








