#lang racket/base

(require parser-tools/lex
        (prefix-in : parser-tools/lex-sre)
        racket/port)


(define-empty-tokens operators
  (Eof
  Lambda Dot
  Opar Cpar))

(define-tokens identifiers
  (Var))

(define-tokens numbers
  (Int))

(define tokenize
  (lexer-src-pos
    ((eof)           (token-Eof))
    (whitespace      (return-without-pos (tokenize input-port)))
    ("lambda"        (token-Lambda))
    ("λ"             (token-Lambda))
    ("."             (token-Dot))
    ("("             (token-Opar))
    (")"             (token-Cpar))
    ((:+ alphabetic) (token-Var lexeme))
    (any-char        (error (format "Unrecognized char '~a' at offset ~a."
                                   lexeme (position-offset start-pos))))))


(define (lex in)
  (port-count-lines! in)
  (let loop
    ((t (tokenize in)))
    (unless (eq? (position-token-token t) 'Eof)
      (write t)
      (newline)
      (loop (tokenize in)))))


(define argv (current-command-line-arguments))
  (cond
    ((= (vector-length argv) 1)
      (call-with-input-file (vector-ref argv 0) lex))
    (else
      (eprintf "Usage: racket lexer.rkt <file>\n")
      (exit 1)))