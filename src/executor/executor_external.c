/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:05:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:30:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	execute_child_process(char *path, char **args, char **envp)
{
	setup_default_signal_handlers();
	execve(path, args, envp);
	if (errno == ENOEXEC)
	{
		args[0] = path;
		execve("/bin/sh", args, envp);
	}
	perror_exit("execve");
}

int	execute_external_command(char *path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		execute_child_process(path, args, envp);
	else if (pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		setup_signal_handlers();
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		return (WEXITSTATUS(status));
	}
	return (1);
}
