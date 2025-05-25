/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mixed_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:38:07 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*find_command_node(t_ast_node *node)
{
	while (node && node->type != NODE_COMMAND)
		node = node->left;
	return (node);
}

t_ast_node	*create_command_with_args(int arg_count)
{
	t_ast_node	*cmd;

	(void)arg_count;
	cmd = create_ast_node(NODE_COMMAND);
	return (cmd);
}

char	**allocate_args_array(int arg_count, t_ast_node *cmd)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		free_ast(cmd);
	return (args);
}

void	cleanup_on_error(char **args, int i, t_ast_node *cmd)
{
	while (--i >= 0)
		free(args[i]);
	free(args);
	free_ast(cmd);
}

void	finalize_command_args(t_ast_node *cmd, char **args, int i)
{
	t_ast_node	*command_node;

	args[i] = NULL;
	command_node = find_command_node(cmd);
	if (command_node)
		command_node->args = args;
	else
		cmd->args = args;
}
