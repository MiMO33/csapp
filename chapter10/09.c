if (Fork() == 0) {
    Dup2(0, 3);
    Execve("fstatcheck", argv, envp);
}