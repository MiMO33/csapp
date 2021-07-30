store_prod:
    movq %rdx, %rax     # move dest to %rax
    cqto                # Convert quadword in %rax to octoword in %rdx:%rax
    movq %rsi, %rcx     # move x to %rcx
    sarq $63, %rcx      # x 算术左移63位
    imulq %rax, %rcx    # 
    imulq %rsi, %rdx
    addq %rdx, %rcx
    mulq %rsi
    addq %rcx, %rdx
    movq %rax, (%rdi)
    movq %rdx, 8(%rdi)
    ret