" vim.vim
" Read stdin and print it

let g:input_line = ""
while !empty(getline("."))
    let g:input_line .= getline(".")
    call append(line("."), "")
endwhile

echo g:input_line
quit!
