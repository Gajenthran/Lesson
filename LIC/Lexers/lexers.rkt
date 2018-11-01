#lang racket/base

(require parser-tools/lex
         (prefix-in : parser-tools/lex-sre)
         racket/port)


(define first-lexer
  (lexer
    ((eof)           'fini)
    (whitespace      (first-lexer input-port))
    (any-char        (begin (display lexeme) (first-lexer input-port)))))


(define second-lexer
  (lexer
    ((eof)           'fini)
    (whitespace      (second-lexer input-port))
    ((:+ alphabetic) lexeme)
    ((:+ numeric)    (string->number lexeme))
    (any-char        lexeme)))


(define (second-lex in)
  (let loop ((t (second-lexer in)))
    (unless (eq? t 'fini)
      (write t)
      (newline)
      (loop (second-lexer in)))))


(call-with-input-string "Est-ce que ça marche ?" first-lexer)

(call-with-input-string "Est-ce que ça remarche ?" second-lex)

(define argv (current-command-line-arguments))
  (cond
    ((= (vector-length argv) 1)
      (call-with-input-file (vector-ref argv 0) second-lex))
    (else
      (eprintf "Usage: racket lexers.rkt <file>\n")
      (exit 1)))


