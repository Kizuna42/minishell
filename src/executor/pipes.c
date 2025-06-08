/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/08 19:26:02 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	setup_pipes(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

void	close_pipes(int pipefd[2])
{
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
}

static int	execute_left_child(t_ast_node *node, t_minishell *shell,
	int pipefd[2])
{
	setup_default_signal_handlers();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_ast(node->left, shell));
}

static int	execute_right_child(t_ast_node *node, t_minishell *shell,
	int pipefd[2])
{
	setup_default_signal_handlers();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_ast(node->right, shell));
}

int	execute_pipeline(t_ast_node *node, t_minishell *shell)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (setup_pipes(pipefd) != 0)
		return (1);
	left_pid = fork();
	if (left_pid == 0)
		execute_left_child(node, shell, pipefd);
	right_pid = fork();
	if (right_pid == 0)
		execute_right_child(node, shell, pipefd);
	close_pipes(pipefd);
	setup_child_signal_handlers();
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	setup_signal_handlers();
	return (WEXITSTATUS(status));
}
