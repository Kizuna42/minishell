/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:05:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:36:18 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	execute_script_file(char *path, t_minishell *shell)
{
	int		fd;
	char	*line;
	int		exit_status;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (1);
	exit_status = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (*line != '\0')
			exit_status = handle_readline_input(shell, line);
		else
			free(line);
		if (exit_status == -1)
			break ;
		line = get_next_line(fd);
	}
	close(fd);
	if (exit_status == -1)
		return (shell->last_exit_status);
	return (exit_status);
}

static void	execute_child_process(char *path, char **args, char **envp,
	t_minishell *shell)
{
	setup_default_signal_handlers();
	execve(path, args, envp);
	if (errno == ENOEXEC)
		exit(execute_script_file(path, shell));
	perror_exit("execve");
}

int	execute_external_command(char *path, char **args, char **envp,
	t_minishell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		execute_child_process(path, args, envp, shell);
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
